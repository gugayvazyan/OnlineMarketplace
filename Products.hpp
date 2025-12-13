#pragma once
#include "OnlineMarketplace.hpp"

enum class Gender { Male, Female, Other };
enum class ClothType { Original, Copy };
enum class Compound { Cotton, Wool, Polyester, Leather };

enum class ElectType { Phone, Laptop, Tablet, TV };
enum class KidsType { Toy, Clothing, Stroller, Other };
enum class HomeType { Furniture, Kitchen, Decor, Other };
enum class BeautyType { Cosmetics, Perfume, Care, Other };

class Electronics : public Product {
    ElectType type;
public:
    Electronics(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, ElectType type);
    void printInfo() const override;
};

class Kids : public Product {
    size_t ageLimit;
    KidsType type;
public:
    Kids(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, size_t ageLimit, KidsType type);
    void printInfo() const override;
};

class Clothing : public Product {
    std::string sizeName;
    ClothType type;
    Compound compound;
    Gender gender;
    bool forKids;
public:
    Clothing(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, std::string sizeName, ClothType type, Compound compound, Gender gender, bool forKids);
    void printInfo() const override;
};

class Home : public Product {
    HomeType type;
public:
    Home(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, HomeType type);
    void printInfo() const override;
};

class Beauty : public Product {
    BeautyType type;
public:
    Beauty(size_t price, size_t warranty, std::string description, User& seller, Warehouse& storage, BeautyType type);
    void printInfo() const override;
};

const char* toString(ElectType t);
const char* toString(KidsType t);
const char* toString(HomeType t);
const char* toString(BeautyType t);
const char* toString(Gender g);
const char* toString(ClothType t);
const char* toString(Compound c);
