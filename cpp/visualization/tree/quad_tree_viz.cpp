#include <SFML/Graphics.hpp>
#include <random>
#include <unordered_map>
#include <vector>

#include "common/geometries/rect.h"
#include "tree/intersect_handler.h"
#include "tree/quad_tree.h"

using namespace utilities;

#define CANVAS_WIDTH 1600
#define CANVAS_HEIGHT 1600
#define QUERY_SIZE 40

size_t global_id = 0;
std::unordered_map<size_t, std::shared_ptr<Shape2D>> objects;
std::unordered_set<size_t> candidates;

int getRandomInt(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}

sf::RenderWindow window(sf::VideoMode(CANVAS_WIDTH, CANVAS_HEIGHT), "QuadTree Visualization");

void drawObj(const std::shared_ptr<Shape2D> &obj, const sf::Color &color) {
  if (obj->GetShapeType() == Shape2DType::RECT) {
    auto *rect = dynamic_cast<Rect *>(obj.get());

    double width = rect->GetWidth();
    double height = rect->GetHeight();

    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setPosition(rect->GetLeftDown().GetX(), rect->GetLeftDown().GetY());
    rectangle.setFillColor(color);

    window.draw(rectangle);
  }
}

void drawObjects() {
  for (auto &[id, obj] : objects) {
    drawObj(obj, candidates.find(id) != candidates.end() ? sf::Color::Blue : sf::Color::Green);
  }
}

void drawLine(const Vector2D &p1, const Vector2D &p2) {
  sf::Vertex line[] = {
      sf::Vertex(sf::Vector2f(p1.GetX(), p1.GetY()), sf::Color::Red),
      sf::Vertex(sf::Vector2f(p2.GetX(), p2.GetY()), sf::Color::Red),
  };

  window.draw(line, 2, sf::Lines);
}

void drawRect(const Rect &rect) {
  auto left_down = rect.GetLeftDown();
  auto right_down = left_down + Vector2D(rect.GetWidth(), 0);
  auto right_up = rect.GetRightUp();
  auto left_up = rect.GetRightUp() - Vector2D(rect.GetWidth(), 0);

  drawLine(left_down, right_down);
  drawLine(right_down, right_up);
  drawLine(right_up, left_up);
  drawLine(left_up, left_down);
}

void refreshTree(QuadTree &tree) {
  std::vector<std::shared_ptr<Shape2D>> objs;
  for (auto &[_, obj] : objects) {
    objs.push_back(obj);
  }
  tree.Build(objs);
}

void drawTree(QuadTree &tree) { tree.Traverse(drawRect); }

void addObject(QuadTree &tree, sf::Vector2i pos) {
  auto left_down = Vector2D(pos.x, pos.y);
  int width = getRandomInt(2, 40);
  int height = getRandomInt(2, 40);
  auto right_up = left_down + Vector2D(width, height);

  auto obj = std::make_shared<Rect>(left_down, right_up);
  obj->id_ = global_id++;
  objects[obj->id_] = obj;

  tree.Insert(obj);
}

void removeObject(QuadTree &tree, sf::Vector2i pos) {
  auto query_point = std::make_shared<Point>(Vector2D(pos.x, pos.y));
  candidates.clear();
  tree.Query(query_point, candidates);

  for (auto id : candidates) {
    if (objects.find(id) != objects.end()) {
      std::cout << "Check Object " << id << std::endl;
      auto &obj = objects[id];
      if (obj->Interact(*query_point)) {
        std::cout << "Remove Object " << id << std::endl;
        tree.Remove(objects[id]);
        objects.erase(id);
      }
    }
  }
}

void detectMouseEvent(sf::Event &event, QuadTree &tree) {
  static bool isMouseLeftPressed = false;
  static bool isMouseRightPressed = false;

  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
    if (!isMouseLeftPressed) {
      isMouseLeftPressed = true;
      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      addObject(tree, mousePosition);
    }
  }

  if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
    isMouseLeftPressed = false;
  }

  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
    if (!isMouseRightPressed) {
      isMouseRightPressed = true;
      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      std::cout << "Try Remove Object" << std::endl;
      removeObject(tree, mousePosition);
    }
  }

  if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
    isMouseRightPressed = false;
  }
}

void drawQuery(const QuadTree &tree) {
  sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

  auto query_point = std::make_shared<Point>(Vector2D(mousePosition.x, mousePosition.y));

  //   auto left_down = mousePosition - sf::Vector2i(QUERY_SIZE, QUERY_SIZE) / 2;
  //   auto right_up = mousePosition + sf::Vector2i(QUERY_SIZE, QUERY_SIZE) / 2;

  //   auto query_rect = std::make_shared<Rect>(Vector2D(left_down.x, left_down.y), Vector2D(right_up.x, right_up.y));
  //   drawObj(query_rect, sf::Color::Blue);

  //   candidates.clear();
  //   tree.Query(query_rect, candidates);

  candidates.clear();
  tree.Query(query_point, candidates);
}

int main() {
  window.setFramerateLimit(60);

  QuadTree tree(1, 5, Rect(Vector2D(0, 0), Vector2D(CANVAS_WIDTH, CANVAS_HEIGHT)));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    detectMouseEvent(event, tree);

    window.clear();

    drawQuery(tree);

    drawObjects();
    drawTree(tree);

    window.display();
  }

  return 0;
}