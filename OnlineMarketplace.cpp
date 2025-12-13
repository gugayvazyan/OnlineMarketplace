#include "OnlineMarketplace.hpp"
#include "Products.hpp"

size_t Counter::UserId = 1;
size_t Counter::OrderId = 1;
size_t Counter::TruckId = 1;

const char* User::toString(UserType t) {
    switch (t) {
        case UserType::Buyer: return "Buyer";
        case UserType::Seller: return "Seller";
        case UserType::Admin: return "Admin";
    }
    return "Unknown";
}

const char* User::toString(Country c) {
    switch (c) {
        case Country::Russia: return "Russia";
        case Country::Armenia: return "Armenia";
        case Country::Georgia: return "Georgia";
        case Country::Belarus: return "Belarus";
        case Country::Kazakhstan: return "Kazakhstan";
        case Country::Ukraine: return "Ukraine";
    }
    return "Unknown";
}

User::User(size_t id_, const std::string& name, UserType type_, Country country_, size_t balance_)
    : id(id_), username(name), type(type_), country(country_), balance(balance_) {}

size_t User::getId() const { return id; }
const std::string& User::getUsername() const { return username; }
UserType User::getType() const { return type; }
Country User::getCountry() const { return country; }
size_t User::getBalance() const { return balance; }

bool User::canPay(size_t amount) const { return balance >= amount; }

void User::pay(size_t amount) {
    if (amount > balance) throw InsufficientFunds("Not enough money");
    balance -= amount;
}

void User::addToCart(Product* product) {
    if (!product) throw NotFound("Null product");
    cart.push_back(product);
}

void User::removeFromCart(Product* product) {
    for (size_t i = 0; i < cart.size(); ++i) {
        if (cart[i] == product) {
            cart.erase(cart.begin() + static_cast<long>(i));
            return;
        }
    }
    throw NotFound("Product not found in cart");
}

const std::vector<Product*>& User::getCart() const { return cart; }

void User::addOrder(Order* order) {
    if (!order) throw NotFound("Null order");
    orders.push_back(order);
}

const std::vector<Order*>& User::getOrders() const { return orders; }

Truck::Truck() { truckId = Counter::TruckId++; }

void Truck::loadOrders(std::vector<Order*>& warehouseOrders) {
    orders = warehouseOrders;
    warehouseOrders.clear();
}

void Truck::deliverAll() {
    for (size_t i = 0; i < orders.size(); ++i) {
        if (orders[i]) {
            std::cout << "Truck " << truckId << " delivered Order " << orders[i]->getId() << "\n";
        }
    }
    orders.clear();
}

Warehouse::Warehouse(Country c) : country(c) {}

Country Warehouse::getCountry() const { return country; }

const char* Warehouse::toString(Country c) { return User::toString(c); }

Product::Product(size_t price_, size_t warranty_, std::string description_, ProductCategory category_, User& seller_, Warehouse& storage_)
    : price(price_), warranty(warranty_), description(std::move(description_)), category(category_), seller(seller_), storage(storage_) {
    if (price == 0) throw BadPrice("Price must be > 0");
    if (description.empty()) throw InvalidName("Description is empty");
}

size_t Product::getPrice() const { return price; }
size_t Product::getWarranty() const { return warranty; }
const std::string& Product::getDescription() const { return description; }
ProductCategory Product::getCategory() const { return category; }
User& Product::getSeller() const { return seller; }
Warehouse& Product::getStorage() const { return storage; }

const char* Product::toString(ProductCategory c) {
    switch (c) {
        case ProductCategory::Electronics: return "Electronics";
        case ProductCategory::Clothing: return "Clothing";
        case ProductCategory::Kids: return "Kids";
        case ProductCategory::Home: return "Home";
        case ProductCategory::Beauty: return "Beauty";
    }
    return "Unknown";
}

void Product::printBaseInfo() const {
    std::cout << "Price: " << price << "\n";
    std::cout << "Warranty: " << warranty << " months\n";
    std::cout << "Description: " << description << "\n";
    std::cout << "Category: " << Product::toString(category) << "\n";
    std::cout << "Seller: " << seller.getUsername() << "\n";
    std::cout << "Warehouse country: " << Warehouse::toString(storage.getCountry()) << "\n";
}

Order::Order(User* user_, const std::vector<Product*>& selectedProducts)
    : orderId(Counter::OrderId++), user(user_), products(selectedProducts), status(OrderStatus::Pending) {
    if (!user) throw NotFound("Order user is null");
    if (products.empty()) throw NotFound("Order has no products");
}

size_t Order::getId() const { return orderId; }
User* Order::getUser() const { return user; }
const std::vector<Product*>& Order::getProducts() const { return products; }
OrderStatus Order::getStatus() const { return status; }

size_t Order::getTotalCost() const {
    size_t total = 0;
    for (size_t i = 0; i < products.size(); ++i) {
        if (products[i]) total += products[i]->getPrice();
    }
    return total;
}

const char* Order::toString(OrderStatus s) {
    switch (s) {
        case OrderStatus::Pending: return "Pending";
        case OrderStatus::Shipped: return "Shipped";
        case OrderStatus::Delivered: return "Delivered";
        case OrderStatus::Canceled: return "Canceled";
    }
    return "Unknown";
}

void Order::printOrder() const {
    std::cout << "Order ID: " << orderId << "\n";
    std::cout << "Status: " << Order::toString(status) << "\n";
    if (user) std::cout << "User: " << user->getUsername() << "\n";

    size_t total = 0;
    for (size_t i = 0; i < products.size(); ++i) {
        Product* p = products[i];
        if (!p) continue;
        std::cout << "-----\n";
        p->printInfo();
        total += p->getPrice();
    }
    std::cout << "-----\n";
    std::cout << "Total cost: " << total << "\n";
}

Delivery::Delivery(Order* order_, Truck* truck_, size_t days_)
    : days(days_), state(OrderStatus::Pending), order(order_), truck(truck_) {
    if (!order) throw NotFound("Delivery order is null");
}

Order* Delivery::getOrder() const { return order; }
Truck* Delivery::getTruck() const { return truck; }
size_t Delivery::getDays() const { return days; }
OrderStatus Delivery::getState() const { return state; }

Electronics::Electronics(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, ElectType type_)
    : Product(price, warranty, std::move(description), ProductCategory::Electronics, seller, storage), type(type_) {}

Kids::Kids(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, size_t ageLimit_, KidsType type_)
    : Product(price, warranty, std::move(description), ProductCategory::Kids, seller, storage), ageLimit(ageLimit_), type(type_) {}

Clothing::Clothing(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, std::string sizeName_,
                   ClothType type_, Compound compound_, Gender gender_, bool forKids_)
    : Product(price, warranty, std::move(description), ProductCategory::Clothing, seller, storage),
      sizeName(std::move(sizeName_)), type(type_), compound(compound_), gender(gender_), forKids(forKids_) {}

Home::Home(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, HomeType type_)
    : Product(price, warranty, std::move(description), ProductCategory::Home, seller, storage), type(type_) {}

Beauty::Beauty(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, BeautyType type_)
    : Product(price, warranty, std::move(description), ProductCategory::Beauty, seller, storage), type(type_) {}

const char* toString(ElectType t) {
    switch (t) {
        case ElectType::Phone: return "Phone";
        case ElectType::Laptop: return "Laptop";
        case ElectType::Tablet: return "Tablet";
        case ElectType::TV: return "TV";
    }
    return "Unknown";
}

const char* toString(KidsType t) {
    switch (t) {
        case KidsType::Toy: return "Toy";
        case KidsType::Clothing: return "Clothing";
        case KidsType::Stroller: return "Stroller";
        case KidsType::Other: return "Other";
    }
    return "Unknown";
}

const char* toString(HomeType t) {
    switch (t) {
        case HomeType::Furniture: return "Furniture";
        case HomeType::Kitchen: return "Kitchen";
        case HomeType::Decor: return "Decor";
        case HomeType::Other: return "Other";
    }
    return "Unknown";
}

const char* toString(BeautyType t) {
    switch (t) {
        case BeautyType::Cosmetics: return "Cosmetics";
        case BeautyType::Perfume: return "Perfume";
        case BeautyType::Care: return "Care";
        case BeautyType::Other: return "Other";
    }
    return "Unknown";
}

const char* toString(Gender g) {
    switch (g) {
        case Gender::Male: return "Male";
        case Gender::Female: return "Female";
        case Gender::Other: return "Other";
    }
    return "Unknown";
}

const char* toString(ClothType t) {
    switch (t) {
        case ClothType::Original: return "Original";
        case ClothType::Copy: return "Copy";
    }
    return "Unknown";
}

const char* toString(Compound c) {
    switch (c) {
        case Compound::Cotton: return "Cotton";
        case Compound::Wool: return "Wool";
        case Compound::Polyester: return "Polyester";
        case Compound::Leather: return "Leather";
    }
    return "Unknown";
}

void Electronics::printInfo() const {
    std::cout << "=== PRODUCT INFO ===\n";
    printBaseInfo();
    std::cout << "Electronics type: " << ::toString(type) << "\n";
}

void Kids::printInfo() const {
    std::cout << "=== PRODUCT INFO ===\n";
    printBaseInfo();
    std::cout << "Kids type: " << ::toString(type) << "\n";
    std::cout << "Age limit: " << ageLimit << "\n";
}

void Clothing::printInfo() const {
    std::cout << "=== PRODUCT INFO ===\n";
    printBaseInfo();
    std::cout << "Clothing size: " << sizeName << "\n";
    std::cout << "Clothing type: " << ::toString(type) << "\n";
    std::cout << "Compound: " << ::toString(compound) << "\n";
    std::cout << "Gender: " << ::toString(gender) << "\n";
    std::cout << "For kids: " << (forKids ? "Yes" : "No") << "\n";
}

void Home::printInfo() const {
    std::cout << "=== PRODUCT INFO ===\n";
    printBaseInfo();
    std::cout << "Home type: " << ::toString(type) << "\n";
}

void Beauty::printInfo() const {
    std::cout << "=== PRODUCT INFO ===\n";
    printBaseInfo();
    std::cout << "Beauty type: " << ::toString(type) << "\n";
}
