#ifndef USER_H
#define USER_H

#include<string>
#include "Cart.h"
using namespace std;

class User {
private:
    int userId;
    string name;
    string address;
    Cart* cart;

public:
    User(int userId, const string& name, const string& address) {
        this->userId = userId;
        this->name = name;
        this->address = address;
        this->cart = new Cart();
    }

    ~User() {
        delete cart; // Clean up the cart when the user is destroyed
    }

    // Getters and setters
    string getName() const {
        return name;
    }

    void setName(const string &n){
        name = n;
    }

    string getAddress() const {
        return address;
    }

    void setAddress(const string &addr){
        address = addr;
    }

    Cart* getCart() const {
        return cart;
    }

    int getUserId() const {
        return userId;
    }

    void setUserId(int id){
        userId = id;
    }

    

    

    

    

};
#endif // USER_H