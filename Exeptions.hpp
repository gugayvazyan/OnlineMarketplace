#pragma once
#include <stdexcept>
#include <string>

class MarketplaceExceptions : public std::runtime_error {
public:
    explicit MarketplaceExceptions(const std::string& msg) : std::runtime_error(msg) {}
};

class BadPrice : public MarketplaceExceptions {
public:
    explicit BadPrice(const std::string& msg) : MarketplaceExceptions(msg) {}
};

class InvalidName : public MarketplaceExceptions {
public:
    explicit InvalidName(const std::string& msg) : MarketplaceExceptions(msg) {}
};

class NotFound : public MarketplaceExceptions {
public:
    explicit NotFound(const std::string& msg) : MarketplaceExceptions(msg) {}
};

class InsufficientFunds : public MarketplaceExceptions {
public:
    explicit InsufficientFunds(const std::string& msg) : MarketplaceExceptions(msg) {}
};

class WrongPassword : public MarketplaceExceptions {
public:
    explicit WrongPassword(const std::string& msg) : MarketplaceExceptions(msg) {}
};
