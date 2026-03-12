#ifndef ORDER_H
#define ORDER_H

#include<iostream>
#include<string>
#include<vector>
#include "User.h"
#include "Restaurant.h"
#include "MenuItem.h"
#include "../strategies/PaymentStrategy.h"
#include "../utils/TimeUtils.h"
using namespace std;

class Order{
protected:
    static int nextOrderID;
    int orderID;
    User* user;
    Restaurant* restaurant;
    vector<MenuItem> items;
    PaymentStrategy* paymentStrategy;
    double total;
    string scheduled;

public:
    Order(){
        user = nullptr;
        restaurant = nullptr;
        paymentStrategy = nullptr;  
        total = 0.0;
        scheduled = "";
        orderID = ++nextOrderID;
    }

    virtual ~Order() {
        delete paymentStrategy; // Clean up the payment strategy when the order is destroyed
    }

    bool processPayment(){
        if(paymentStrategy){
            paymentStrategy->pay(total);
            return true;
        }else{
            cout<<"Please choose a payment mode first"<<endl;
            return false;
        }
    }

    virtual string getType() const = 0;

    // Getters and setters

    int getOrderId() const{
        return orderID;
    }

    void setUser(User* u){
        user = u;
    }

    User* getUser() const {
        return user;
    }

    void setRestaurant(Restaurant* r){
        restaurant = r;
    }

    Restaurant* getRestaurant() const {
        return restaurant;
    }

    void setItems(const vector<MenuItem>& its){
        items = its;
        total = 0;
        for(auto &i : items){
            total += i.getPrice();  
        }
    }

    const vector<MenuItem>& getItems() const {
        return items;
    }

    void setPaymentStrategy(PaymentStrategy* p){
        paymentStrategy = p;
    }

    PaymentStrategy* getPaymentStrategy() const {
        return paymentStrategy;
    }

    double getTotal() const { return total; }
    void setTotal(double t) { total = t; }
    string getScheduled() const { return scheduled; }
    void setScheduledTime(const string& s) { scheduled = s; }

};
int Order::nextOrderID = 0;
#endif // ORDER_H
