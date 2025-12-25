#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Products.hpp"

class Management {
    std::vector<std::unique_ptr<User>> users;
    std::vector<std::unique_ptr<Product>> products;
    std::vector<std::unique_ptr<Order>> orders;
    std::vector<std::unique_ptr<Delivery>> deliveries;

    Warehouse russiaWarehouse;
    Warehouse armeniaWarehouse;
    Warehouse georgiaWarehouse;
    Warehouse belarusWarehouse;
    Warehouse kazakhstanWarehouse;
    Warehouse ukraineWarehouse;

    void bootstrap();

public:
    Management();
    ~Management() = default;

    User* createUser(const std::string& name,
                     UserType type,
                     Country country,
                     size_t balance = 0,
                     const std::string& password = "");

    void addProduct(std::unique_ptr<Product> product);

    Order* createOrder(User* user, const std::vector<Product*>& selectedProducts);
    Delivery* createDelivery(Order* order);

    Warehouse* getWarehouse(Country country);
    Truck& getTruck(Warehouse& warehouse);

    const std::vector<std::unique_ptr<Product>>& getAllProducts() const;
    const std::vector<std::unique_ptr<User>>& getAllUsers() const;

    void run();
    User* pickUser(UserType type);
    void runUser(User* u);

    void buyerMenu(User* buyer);
    void sellerMenu(User* seller);
    void adminMenu(User* admin);
};
