#include "Marketplace.hpp"

Management::Management()
    : russiaWarehouse(Country::Russia),
      armeniaWarehouse(Country::Armenia),
      georgiaWarehouse(Country::Georgia),
      belarusWarehouse(Country::Belarus),
      kazakhstanWarehouse(Country::Kazakhstan),
      ukraineWarehouse(Country::Ukraine) {
    bootstrap();
}

void Management::bootstrap() {
    
    for (int i = 0; i < 2; ++i) armeniaWarehouse.garage.push_back(std::make_unique<Truck>());
    for (int i = 0; i < 2; ++i) georgiaWarehouse.garage.push_back(std::make_unique<Truck>());
    for (int i = 0; i < 2; ++i) belarusWarehouse.garage.push_back(std::make_unique<Truck>());
    for (int i = 0; i < 2; ++i) kazakhstanWarehouse.garage.push_back(std::make_unique<Truck>());
    for (int i = 0; i < 2; ++i) russiaWarehouse.garage.push_back(std::make_unique<Truck>());
    for (int i = 0; i < 2; ++i) ukraineWarehouse.garage.push_back(std::make_unique<Truck>());


    User* sellerAM = createUser("Seller_Armenia", UserType::Seller, Country::Armenia, 0, "1234");
    User* sellerGE = createUser("Seller_Georgia", UserType::Seller, Country::Georgia, 0, "1234");
    User* sellerBY = createUser("Seller_Belarus", UserType::Seller, Country::Belarus, 0, "1234");
    User* sellerKZ = createUser("Seller_Kazakhstan", UserType::Seller, Country::Kazakhstan, 0, "1234");
    User* sellerRU = createUser("Seller_Russia", UserType::Seller, Country::Russia, 0, "1234");

    Warehouse* whAM = getWarehouse(Country::Armenia);
    Warehouse* whGE = getWarehouse(Country::Georgia);
    Warehouse* whBY = getWarehouse(Country::Belarus);
    Warehouse* whKZ = getWarehouse(Country::Kazakhstan);
    Warehouse* whRU = getWarehouse(Country::Russia);

    for (int k = 0; k < 3; ++k) {
        addProduct(std::make_unique<Electronics>(1200 + k * 200, 12, "AM Electronics #" + std::to_string(k + 1), *sellerAM, *whAM, ElectType::Phone));
    }

    for (int k = 0; k < 3; ++k) {
        addProduct(std::make_unique<Kids>(400 + k * 80, 6, "GE Kids #" + std::to_string(k + 1), *sellerGE, *whGE, 6 + static_cast<size_t>(k), KidsType::Toy));
    }

    for (int k = 0; k < 3; ++k) {
        addProduct(std::make_unique<Home>(700 + k * 150, 12, "BY Home #" + std::to_string(k + 1), *sellerBY, *whBY, HomeType::Furniture));
    }

    for (int k = 0; k < 3; ++k) {
        addProduct(std::make_unique<Clothing>(500 + k * 120, 3, "KZ Clothing #" + std::to_string(k + 1), *sellerKZ, *whKZ, "M", ClothType::Original, Compound::Cotton, Gender::Male, false));
    }

    for (int k = 0; k < 3; ++k) {
        addProduct(std::make_unique<Beauty>(600 + k * 110, 6, "RU Beauty #" + std::to_string(k + 1), *sellerRU, *whRU, BeautyType::Cosmetics));
    }
}

User* Management::createUser(const std::string& name, UserType type, Country country, size_t balance, const std::string& password) {
    const size_t id = Counter::UserId++;
    users.push_back(std::make_unique<User>(id, name, type, country, balance, password));
    return users.back().get();
}

void Management::addProduct(std::unique_ptr<Product> product) {
    if (!product) return;
    products.push_back(std::move(product));
}

Order* Management::createOrder(User* user, const std::vector<Product*>& selectedProducts) {
    if (!user) throw NotFound("User is null");
    if (selectedProducts.empty()) throw NotFound("No products selected");

    for (size_t i = 0; i < selectedProducts.size(); ++i) {
        if (!selectedProducts[i]) throw NotFound("Null product in selection");
    }

    orders.push_back(std::make_unique<Order>(user, selectedProducts));
    Order* ord = orders.back().get();
    user->addOrder(ord);

    Warehouse& wh = selectedProducts[0]->getStorage();
    wh.orders.push_back(ord);

    return ord;
}

Delivery* Management::createDelivery(Order* order) {
    if (!order) throw NotFound("Order is null");

    const auto& items = order->getProducts();
    if (items.empty()) throw NotFound("Order has no products");

    Warehouse& wh = items[0]->getStorage();

    Truck& tr = getTruck(wh);
    size_t days = 3;

    deliveries.push_back(std::make_unique<Delivery>(order, &tr, days));
    return deliveries.back().get();
}


Warehouse* Management::getWarehouse(Country country) {
    switch (country) {
        case Country::Russia: return &russiaWarehouse;
        case Country::Armenia: return &armeniaWarehouse;
        case Country::Georgia: return &georgiaWarehouse;
        case Country::Belarus: return &belarusWarehouse;
        case Country::Kazakhstan: return &kazakhstanWarehouse;
        case Country::Ukraine: return &ukraineWarehouse;
    }
    return &armeniaWarehouse;
}

Truck& Management::getTruck(Warehouse& warehouse) {
    if (warehouse.garage.empty()) {
        warehouse.garage.push_back(std::make_unique<Truck>());
    }
    return *warehouse.garage.back();
}


User* Management::pickUser(UserType type) {
    while (true) {
        std::cout << "\n1) Create\n";
        std::cout << "2) Select by id\n";
        std::cout << "3) List id\n";
        std::cout << "0) Back\n";
        std::cout << "Choose: ";

        int cmd;
        std::cin >> cmd;

        if (cmd == 0) return nullptr;

        if (cmd == 1) {
            std::string name;
            int c;
            size_t bal = 0;
            std::string pass;

            std::cout << "Name: ";
            std::cin >> name;

            std::cout << "Country (0 Russia,1 Armenia,2 Georgia,3 Belarus,4 Kazakhstan,5 Ukraine): ";
            std::cin >> c;

            Country country = Country::Armenia;
            if (c == 0) country = Country::Russia;
            else if (c == 1) country = Country::Armenia;
            else if (c == 2) country = Country::Georgia;
            else if (c == 3) country = Country::Belarus;
            else if (c == 4) country = Country::Kazakhstan;
            else if (c == 5) country = Country::Ukraine;

            if (type == UserType::Buyer) {
                std::cout << "Balance: ";
                std::cin >> bal;
            }

            std::cout << "Password: ";
            std::cin >> pass;

            User* u = createUser(name, type, country, bal, pass);

            std::cout << "Created: id=" << u->getId()
                      << " name=" << u->getUsername()
                      << " type=" << User::toString(u->getType())
                      << " country=" << User::toString(u->getCountry())
                      << " balance=" << u->getBalance() << "\n";

            return u;
        }

        if (cmd == 2) {
    size_t id;
    std::cout << "Enter id: ";
    std::cin >> id;

    const auto& us = getAllUsers();
    for (size_t i = 0; i < us.size(); ++i) {
        if (!us[i]) continue;
        if (us[i]->getId() == id && us[i]->getType() == type) {
            User* u = us[i].get();

            for (int attempt = 1; attempt <= 3; ++attempt) {
                std::string passwd;
                std::cout << "Enter password (" << attempt << "/3): ";
                std::cin >> passwd;

                if (passwd == u->getPassword()) {
                    std::cout << "Selected: id=" << u->getId()
                              << " name=" << u->getUsername()
                              << " country=" << User::toString(u->getCountry())
                              << " balance=" << u->getBalance() << std::endl;
                    return u;
                }

                if (attempt < 3) {
                    std::cout << "Wrong password. Try again.\n";
                }
            }

            std::cout << "Wrong password 3 times. Back to main menu.\n";
            return nullptr;
        }
    }

    std::cout << "Not found" << std::endl;
}


        if (cmd == 3) {
            const auto& us = getAllUsers();
            bool any = false;

            std::cout << "\n=== USERS LIST (" << User::toString(type) << ") ===\n";
            for (size_t i = 0; i < us.size(); ++i) {
                if (!us[i]) continue;
                if (us[i]->getType() != type) continue;

                any = true;
                std::cout << "id=" << us[i]->getId()
                          << " name=" << us[i]->getUsername()
                          << " country=" << User::toString(us[i]->getCountry());

                if (type == UserType::Buyer) {
                    std::cout << " balance=" << us[i]->getBalance();
                }
                std::cout << "\n";
            }

            if (!any) {
                std::cout << "No users of this type yet.\n";
            }
        }
    }
}

void Management::runUser(User* u) {
    if (!u) return;

    if (u->getType() == UserType::Buyer) buyerMenu(u);
    else if (u->getType() == UserType::Seller) sellerMenu(u);
    else adminMenu(u);
}

void Management::run() {
    while (true) {
        std::cout << "\n=== START ===\n";
        std::cout << "1) Buyer\n";
        std::cout << "2) Seller\n";
        std::cout << "3) Admin\n";
        std::cout << "0) Exit\n";
        std::cout << "Choose: ";

        int role;
        std::cin >> role;

        if (role == 0) return;

        if (role == 1) {
            User* u = pickUser(UserType::Buyer);
            runUser(u);
        }
        else if (role == 2) {
            User* u = pickUser(UserType::Seller);
            runUser(u);
        }
        else if (role == 3) {
            User* u = pickUser(UserType::Admin);
            runUser(u);
        }
        else {
            std::cout << "Unknown" << std::endl;
        }
    }
}

void Management::buyerMenu(User* buyer) {
    while (true) {
        std::cout << "\n=== BUYER: " << buyer->getUsername() << " | balance=" << buyer->getBalance() << " ===\n";
        std::cout << "1) Show products\n";
        std::cout << "2) Add product to cart (by product index)\n";
        std::cout << "3) Show cart\n";
        std::cout << "4) Remove from cart (by cart index)\n";
        std::cout << "5) Checkout (all cart)\n";
        std::cout << "6) Show my orders\n";
        std::cout << "0) Back\n";
        std::cout << "Choose: ";

        int cmd;
        std::cin >> cmd;

        if (cmd == 0) return;

        try {
            if (cmd == 1) {
                const auto& ps = getAllProducts();
                for (size_t i = 0; i < ps.size(); ++i) {
                    if (!ps[i]) continue;
                    std::cout << "[" << i << "] "
                              << ps[i]->getDescription()
                              << " | price=" << ps[i]->getPrice()
                              << " | cat=" << Product::toString(ps[i]->getCategory())
                              << " | seller=" << ps[i]->getSeller().getUsername()
                              << " | wh=" << Warehouse::toString(ps[i]->getStorage().getCountry())
                              << std::endl;
                }
            }
            else if (cmd == 2) {
                size_t idx;
                std::cout << "Product index: ";
                std::cin >> idx;

                const auto& ps = getAllProducts();
                if (idx >= ps.size() || !ps[idx]) throw NotFound("Bad product index");

                buyer->addToCart(ps[idx].get());
                std::cout << "Added" << std::endl;
            }
            else if (cmd == 3) {
                const auto& cart = buyer->getCart();
                if (cart.empty()) {
                    std::cout << "(cart empty)" << std::endl;
                } else {
                    for (size_t i = 0; i < cart.size(); ++i) {
                        if (!cart[i]) continue;
                        std::cout << "(" << i << ") "
                                  << cart[i]->getDescription()
                                  << " | price=" << cart[i]->getPrice()
                                  << " | wh=" << Warehouse::toString(cart[i]->getStorage().getCountry())
                                  << std::endl;
                    }
                }
            }
            else if (cmd == 4) {
                const auto& cart = buyer->getCart();
                if (cart.empty()) throw NotFound("Cart empty");

                size_t cidx;
                std::cout << "Cart index: ";
                std::cin >> cidx;

                if (cidx >= cart.size() || !cart[cidx]) throw NotFound("Bad cart index");
                buyer->removeFromCart(cart[cidx]);
                std::cout << "Removed" << std::endl;
            }
            else if (cmd == 5) {
    const auto& cart = buyer->getCart();
    if (cart.empty()) throw NotFound("Cart empty");

    std::vector<Product*> selected;
    for (size_t i = 0; i < cart.size(); ++i) {
        if (cart[i]) selected.push_back(cart[i]);
    }

    // Pay in checkout
    size_t total = 0;
    for (size_t i = 0; i < selected.size(); ++i) {
        total += selected[i]->getPrice();
    }
    if (!buyer->canPay(total)) throw InsufficientFunds("Not enough money");
    buyer->pay(total);

    Order* ord = createOrder(buyer, selected);

    // Clear cart after successful purchase
    buyer->clearCart();

    Delivery* del = createDelivery(ord);

    std::cout << "Order created: id=" << ord->getId()
              << " total=" << ord->getTotalCost()
              << " balance=" << buyer->getBalance() << "\n";

    if (del && del->getTruck()) {
        std::cout << "Delivery: truck=" << del->getTruck()->truckId
                  << " days=" << del->getDays() << std::endl;
    }
}
else if (cmd == 6) {
                const auto& os = buyer->getOrders();
                if (os.empty()) {
                    std::cout << "(no orders)" << std::endl;
                } else {
                    for (size_t i = 0; i < os.size(); ++i) {
                        if (!os[i]) continue;
                        std::cout << "Order id=" << os[i]->getId()
                                  << " total=" << os[i]->getTotalCost()
                                  << " status=" << Order::toString(os[i]->getStatus())
                                  << std::endl;
                    }
                }
            }
            else {
                std::cout << "Unknown" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

void Management::sellerMenu(User* seller) {
    while (true) {
        std::cout << "\n=== SELLER: " << seller->getUsername() << " | country=" << User::toString(seller->getCountry()) << " ===\n";
        std::cout << "1) Show my products\n";
        std::cout << "2) Add new product\n";
        std::cout << "3) Show orders with my products\n";
        std::cout << "0) Back\n";
        std::cout << "Choose: ";

        int cmd;
        std::cin >> cmd;

        if (cmd == 0) return;

        try {
            if (cmd == 1) {
                const auto& ps = getAllProducts();
                bool any = false;

                for (size_t i = 0; i < ps.size(); ++i) {
                    if (!ps[i]) continue;
                    if (&ps[i]->getSeller() != seller) continue;

                    any = true;
                    std::cout << "[" << i << "] "
                              << ps[i]->getDescription()
                              << " | price=" << ps[i]->getPrice()
                              << " | cat=" << Product::toString(ps[i]->getCategory())
                              << " | wh=" << Warehouse::toString(ps[i]->getStorage().getCountry())
                              << "\n";
                }

                if (!any) std::cout << "(no products)\n";
            }
            else if (cmd == 2) {
                size_t price, warranty;
                std::string desc;
                int cat;

                std::cout << "Category (0 Electronics,1 Clothing,2 Kids,3 Home,4 Beauty): ";
                std::cin >> cat;

                std::cout << "Price: ";
                std::cin >> price;

                std::cout << "Warranty (months): ";
                std::cin >> warranty;

                std::cout << "Description (one word or use_underscores): ";
                std::cin >> desc;

                Warehouse* wh = getWarehouse(seller->getCountry());
                if (!wh) throw NotFound("Warehouse not found");

                if (cat == 0) {
                    int t;
                    std::cout << "ElectType (0 Phone,1 Laptop,2 Tablet,3 TV): ";
                    std::cin >> t;

                    ElectType et = ElectType::Phone;
                    if (t == 0) et = ElectType::Phone;
                    else if (t == 1) et = ElectType::Laptop;
                    else if (t == 2) et = ElectType::Tablet;
                    else if (t == 3) et = ElectType::TV;

                    addProduct(std::make_unique<Electronics>(price, warranty, desc, *seller, *wh, et));
                }
                else if (cat == 1) {
                    std::string sizeName;
                    int t, comp, gen, kids;

                    std::cout << "Size (S/M/L/XL or any): ";
                    std::cin >> sizeName;

                    std::cout << "ClothType (0 Original,1 Copy): ";
                    std::cin >> t;

                    std::cout << "Compound (0 Cotton,1 Wool,2 Polyester,3 Leather): ";
                    std::cin >> comp;

                    std::cout << "Gender (0 Male,1 Female,2 Other): ";
                    std::cin >> gen;

                    std::cout << "For kids? (0 No,1 Yes): ";
                    std::cin >> kids;

                    ClothType ct = (t == 1) ? ClothType::Copy : ClothType::Original;

                    Compound cc = Compound::Cotton;
                    if (comp == 0) cc = Compound::Cotton;
                    else if (comp == 1) cc = Compound::Wool;
                    else if (comp == 2) cc = Compound::Polyester;
                    else if (comp == 3) cc = Compound::Leather;

                    Gender gg = Gender::Male;
                    if (gen == 0) gg = Gender::Male;
                    else if (gen == 1) gg = Gender::Female;
                    else if (gen == 2) gg = Gender::Other;

                    bool fk = (kids == 1);

                    addProduct(std::make_unique<Clothing>(price, warranty, desc, *seller, *wh, sizeName, ct, cc, gg, fk));
                }
                else if (cat == 2) {
                    size_t age;
                    int t;

                    std::cout << "Age limit: ";
                    std::cin >> age;

                    std::cout << "KidsType (0 Toy,1 Clothing,2 Stroller,3 Other): ";
                    std::cin >> t;

                    KidsType kt = KidsType::Toy;
                    if (t == 0) kt = KidsType::Toy;
                    else if (t == 1) kt = KidsType::Clothing;
                    else if (t == 2) kt = KidsType::Stroller;
                    else if (t == 3) kt = KidsType::Other;

                    addProduct(std::make_unique<Kids>(price, warranty, desc, *seller, *wh, age, kt));
                }
                else if (cat == 3) {
                    int t;
                    std::cout << "HomeType (0 Furniture,1 Kitchen,2 Decor,3 Other): ";
                    std::cin >> t;

                    HomeType ht = HomeType::Furniture;
                    if (t == 0) ht = HomeType::Furniture;
                    else if (t == 1) ht = HomeType::Kitchen;
                    else if (t == 2) ht = HomeType::Decor;
                    else if (t == 3) ht = HomeType::Other;

                    addProduct(std::make_unique<Home>(price, warranty, desc, *seller, *wh, ht));
                }
                else if (cat == 4) {
                    int t;
                    std::cout << "BeautyType (0 Cosmetics,1 Perfume,2 Care,3 Other): ";
                    std::cin >> t;

                    BeautyType bt = BeautyType::Cosmetics;
                    if (t == 0) bt = BeautyType::Cosmetics;
                    else if (t == 1) bt = BeautyType::Perfume;
                    else if (t == 2) bt = BeautyType::Care;
                    else if (t == 3) bt = BeautyType::Other;

                    addProduct(std::make_unique<Beauty>(price, warranty, desc, *seller, *wh, bt));
                }
                else {
                    std::cout << "Unknown category" << std::endl;
                    continue;
                }

                std::cout << "Product added\n";
            }
            else if (cmd == 3) {
                bool any = false;

                for (size_t i = 0; i < orders.size(); ++i) {
                    if (!orders[i]) continue;

                    Order* o = orders[i].get();
                    const auto& items = o->getProducts();

                    bool mine = false;
                    for (size_t j = 0; j < items.size(); ++j) {
                        if (!items[j]) continue;
                        if (&items[j]->getSeller() == seller) {
                            mine = true;
                            break;
                        }
                    }

                    if (mine) {
                        any = true;
                        std::cout << "Order id=" << o->getId()
                                  << " total=" << o->getTotalCost()
                                  << " status=" << Order::toString(o->getStatus())
                                  << " buyer=" << (o->getUser() ? o->getUser()->getUsername() : "null")
                                  << std::endl;
                    }
                }

                if (!any) std::cout << "(no orders)\n";
            }
            else {
                std::cout << "Unknown\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

void Management::adminMenu(User* admin) {
    while (true) {
        std::cout << "\n=== ADMIN: " << admin->getUsername() << " ===\n";
        std::cout << "1) Show products\n";
        std::cout << "2) Deliver warehouse orders\n";
        std::cout << "3) Switch to Buyer interface\n";
        std::cout << "4) Switch to Seller interface\n";
        std::cout << "0) Back\n";
        std::cout << "Choose: ";

        int cmd;
        std::cin >> cmd;

        if (cmd == 0) return;

        try {
            if (cmd == 1) {
                const auto& ps = getAllProducts();
                for (size_t i = 0; i < ps.size(); ++i) {
                    if (!ps[i]) continue;
                    std::cout << "[" << i << "] "
                              << ps[i]->getDescription()
                              << " | price=" << ps[i]->getPrice()
                              << " | cat=" << Product::toString(ps[i]->getCategory())
                              << " | seller=" << ps[i]->getSeller().getUsername()
                              << " | wh=" << Warehouse::toString(ps[i]->getStorage().getCountry())
                              << std::endl;
                }
            }
            else if (cmd == 2) {
                int c;
                std::cout << "Warehouse country (0 Russia,1 Armenia,2 Georgia,3 Belarus,4 Kazakhstan,5 Ukraine): ";
                std::cin >> c;

                Country country = Country::Armenia;
                if (c == 0) country = Country::Russia;
                else if (c == 1) country = Country::Armenia;
                else if (c == 2) country = Country::Georgia;
                else if (c == 3) country = Country::Belarus;
                else if (c == 4) country = Country::Kazakhstan;
                else if (c == 5) country = Country::Ukraine;

                Warehouse* wh = getWarehouse(country);
                if (!wh) throw NotFound("Warehouse not found");

                if (wh->orders.empty()) {
                    std::cout << "(warehouse has no orders)\n";
                } else {
                    Truck& tr = getTruck(*wh);
                    tr.loadOrders(wh->orders);
                    tr.deliverAll();
                }
            }
            else if (cmd == 3) {
                User* u = pickUser(UserType::Buyer);
                runUser(u);
            }
            else if (cmd == 4) {
                User* u = pickUser(UserType::Seller);
                runUser(u);
            }
            else {
                std::cout << "Unknown\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

const std::vector<std::unique_ptr<Product>>& Management::getAllProducts() const {
    return products;
}

const std::vector<std::unique_ptr<User>>& Management::getAllUsers() const {
    return users;
}

