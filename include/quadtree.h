//
// Created by Kevin Gori on 07/03/2021.
//

#pragma once
#include <memory>

struct RectangleBounds {
    RectangleBounds(float xmin_, float xmax_, float ymin_, float ymax_)
            : xmin(xmin_), xmax(xmax_), ymin(ymin_), ymax(ymax_) {}
    float xmin;
    float xmax;
    float ymin;
    float ymax;
};

float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

template <typename T>
class Quadtree {
public:
    Quadtree(float xmin_, float xmax_, float ymin_, float ymax_)
            : xmin(xmin_), xmax(xmax_), ymin(ymin_), ymax(ymax_) {
    }

    void add(T item) {
        if (not isLeaf()) {
            addToChild(item);
        }
        else {
            if (items.size() < 4) {
                items.push_back(item);
            }
            else {
                initialiseChildren();
                for (auto& existingItem : items) {
                    addToChild(existingItem);
                }
                addToChild(item);
            }
        }
    }

    float getWidth() { return xmax - xmin; }
    float getHeight() { return ymax - ymin; }
    RectangleBounds getRectBounds() { return RectangleBounds(xmin, xmax, ymin, ymax); }

    std::vector<RectangleBounds> getAllRectangleBounds() {
        std::vector<RectangleBounds> output;
        pushRectangleBounds(output);
        return output;
    }

    std::vector<RectangleBounds> getIntersectingRectangleBounds(float x, float y, float r) {
        std::vector<RectangleBounds> output;
        pushRectangleBounds(output, x, y, r);
        return output;
    }

    /// Recursively fetches all points at this node and below that fall within the circle
    /// with the given centre and radius
    std::vector<T> getPointsWithinCircle(float centre_x, float centre_y, float radius) {
        std::vector<T> result;
        accumulatePointsWithinCircle(result, centre_x, centre_y, radius);
        return result;
    }

    /// Checks if this Quadtree node's bounding rect intersects the circle with the given centre and radius
    bool intersectsCircle(float centre_x, float centre_y, float radius) {
        if (centre_x >= xmin and centre_x < xmax and centre_y >= ymin and centre_y < ymax) {
            return true;
        }
        float closestX = clamp(centre_x, xmin, xmax);
        float closestY = clamp(centre_y, ymin, ymax);
        float distanceX = centre_x - closestX;
        float distanceY = centre_y - closestY;
        return distanceX * distanceX + distanceY * distanceY < radius * radius;
    }

private:

    // Bounds
    float xmin;
    float xmax;
    float ymin;
    float ymax;

    // Children
    std::unique_ptr<Quadtree> topLeft;
    std::unique_ptr<Quadtree> topRight;
    std::unique_ptr<Quadtree> bottomLeft;
    std::unique_ptr<Quadtree> bottomRight;

    // Contents
    std::vector<T> items;

    bool isLeaf() { return topLeft == nullptr; }

    void initialiseChildren() {
        topLeft = std::make_unique<Quadtree>(xmin, (xmin + xmax) / 2, ymin, (ymin+ymax) / 2);
        topRight = std::make_unique<Quadtree>((xmin + xmax) / 2, xmax, ymin, (ymin+ymax) / 2);
        bottomLeft = std::make_unique<Quadtree>(xmin, (xmin + xmax) / 2, (ymin+ymax) / 2, ymax);
        bottomRight = std::make_unique<Quadtree>((xmin + xmax) / 2, xmax, (ymin+ymax) / 2, ymax);
    }

    void addToChild(T& item) {
        bool isLeft = item->get_position().x < (xmax + xmin) / 2;
        bool isTop = item->get_position().y < (ymax + ymin) / 2;
        if (isLeft) {
            if (isTop) {
                topLeft->add(item);
            } else {
                bottomLeft->add(item);
            }
        } else {
            if (isTop) {
                topRight->add(item);
            } else {
                bottomRight->add(item);
            }
        }
    }

    void pushRectangleBounds(std::vector<RectangleBounds>& acc) {
        if (isLeaf()) {
            acc.push_back(getRectBounds());
        }
        else {
            topLeft->pushRectangleBounds(acc);
            topRight->pushRectangleBounds(acc);
            bottomLeft->pushRectangleBounds(acc);
            bottomRight->pushRectangleBounds(acc);
        }
    }

    void accumulatePointsWithinCircle(std::vector<T>& acc, float centre_x, float centre_y, float radius) {
        if (isLeaf()) {
            if (intersectsCircle(centre_x, centre_y, radius)) {
                for (auto& point : items) {
                    auto pos = point->get_position();
                    if (((pos.x - centre_x) * (pos.x - centre_x) + (pos.y - centre_y) * (pos.y - centre_y)) < radius * radius) {
                        acc.push_back(point);
                    }
                }
            }
        }
        else {
            topLeft->accumulatePointsWithinCircle(acc, centre_x, centre_y, radius);
            topRight->accumulatePointsWithinCircle(acc, centre_x, centre_y, radius);
            bottomLeft->accumulatePointsWithinCircle(acc, centre_x, centre_y, radius);
            bottomRight->accumulatePointsWithinCircle(acc, centre_x, centre_y, radius);
        }
    }

    void pushRectangleBounds(std::vector<RectangleBounds>& acc, float x, float y, float r) {
        if (isLeaf()) {
            if (intersectsCircle(x, y, r)) {
                acc.push_back(getRectBounds());
            }
        }
        else {
            topLeft->pushRectangleBounds(acc, x, y, r);
            topRight->pushRectangleBounds(acc, x, y, r);
            bottomLeft->pushRectangleBounds(acc, x, y, r);
            bottomRight->pushRectangleBounds(acc, x, y, r);
        }
    }
};
