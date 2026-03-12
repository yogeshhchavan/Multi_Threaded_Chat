#include<iostream>
#include "TomatoApp.h"
using namespace std;

int main(){
    // create Tomato facade
    TomatoApp* tomato = new TomatoApp();

    //simlate a user coming in (Happy Flow)
    User* user = new User(101, "Aditya", "Delhi");
    cout<<"User: "<< user->getName() << "is active. "<<endl;

    //User searches for restaurant by location
    vector<Restaurant*> restaurantList = tomato->searchRestaurants("Delhi");

    if(restaurantList.empty()){
        cout<<"No restaurants found."<<endl;
        return 0;
    } 
    cout<<"Found Restaurant : "<<endl;
    for(auto restuarant : restaurantList){
        cout<<" - "<<restuarant->getName()<<endl;
    }

    //User selects a restaurant
    tomato->selectRestaurant(user, restaurantList[0]);
    cout<<"Selected Restauarant : "<<restaurantList[0]->getName()<<endl;

    //User adds items to cart
    tomato->addToCart(user, "P1");
    tomato->addToCart(user, "P2");

    tomato->printUserCart(user);

    // usercheckout the cart
    Order* order = tomato->checkoutNow(user, "Delivery", new UpiPaymentStrategy("1234567890"));

    // user pay for the cart. If payment is success, notification is sent.
    tomato->payForOrder(user, order);

    //cleanup code.
    delete tomato;
    delete user;
    return 0;
}