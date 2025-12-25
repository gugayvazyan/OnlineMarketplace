#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Exeptions.hpp"

class Product;
class Order;
class User;
class Truck;
class Warehouse;
class Management;

enum class UserType { Buyer, Seller, Admin };
enum class ProductCategory { Electronics, Clothing, Kids, Home, Beauty };
enum class OrderStatus { Pending, Shipped, Delivered, Canceled };
enum class Country { Russia, Armenia, Georgia, Belarus, Kazakhstan, Ukraine };

struct Counter {
    static size_t UserId;
    static size_t OrderId;
    static size_t TruckId;
};

class User {
    size_t id{};
    std::string username;
    std::string password;
    UserType type{UserType::Buyer};
    Country country{Country::Armenia};
    size_t balance{0};

    std::vector<Product*> cart;
    std::vector<Order*> orders;

public:
    User(size_t id, const std::string& name, UserType type, Country country, size_t balance, const std::string& password);

    size_t getId() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;
    UserType getType() const;
    Country getCountry() const;
    size_t getBalance() const;

    bool canPay(size_t amount) const;
    void pay(size_t amount);

    void addToCart(Product* product);
    void removeFromCart(Product* product);
    const std::vector<Product*>& getCart() const;
    void clearCart();

    void addOrder(Order* order);
    const std::vector<Order*>& getOrders() const;

    static const char* toString(UserType t);
    static const char* toString(Country c);
};

class Truck {
public:
    size_t truckId{};
    std::vector<Order*> orders;

    Truck();
    void loadOrders(std::vector<Order*>& warehouseOrders);
    void deliverAll();
};

class Warehouse {
    Country country{Country::Armenia};

public:
    std::vector<Order*> orders;
    std::vector<std::unique_ptr<Truck>> garage;

    explicit Warehouse(Country c = Country::Armenia);

    Country getCountry() const;

    static const char* toString(Country c);
};

class Product {
protected:
    size_t price{0};
    size_t warranty{0};
    std::string description;
    ProductCategory category{ProductCategory::Electronics};
    User& seller;
    Warehouse& storage;

public:
    Product(size_t price, size_t warranty, std::string description, ProductCategory category, User& seller, Warehouse& storage);
    virtual ~Product() = default;

    size_t getPrice() const;
    size_t getWarranty() const;
    const std::string& getDescription() const;
    ProductCategory getCategory() const;
    User& getSeller() const;
    Warehouse& getStorage() const;

    virtual void printInfo() const = 0;

    static const char* toString(ProductCategory c);

protected:
    void printBaseInfo() const;
};

class Order {
    size_t orderId{};
    User* user{nullptr};
    std::vector<Product*> products;
    OrderStatus status{OrderStatus::Pending};

public:
    Order(User* user, const std::vector<Product*>& selectedProducts);

    size_t getId() const;
    User* getUser() const;
    const std::vector<Product*>& getProducts() const;
    size_t getTotalCost() const;
    OrderStatus getStatus() const;

    void printOrder() const;

    static const char* toString(OrderStatus s);
};

class Delivery {
    size_t days{0};
    OrderStatus state{OrderStatus::Pending};
    Order* order{nullptr};
    Truck* truck{nullptr};

public:
    Delivery(Order* order, Truck* truck, size_t days);

    Order* getOrder() const;
    Truck* getTruck() const;
    size_t getDays() const;
    OrderStatus getState() const;
};
