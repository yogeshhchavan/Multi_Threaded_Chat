#ifndef RESTAURANT_MANAGER_H
#define RESTAURANT_MANAGER_H

#include<vector>
#include<string>
#include<algorithm>
#include "../models/Restaurant.h"
using namespace std;

class RestaurantManager {
private:
    vector<Restaurant*> restaurants;
    static RestaurantManager* instance;
    RestaurantManager() {
        // Private constructor 
    } 

public:
    static RestaurantManager* getInstance() {
        if (!instance) {
            instance = new RestaurantManager();
        }
        return instance;
    }

    void addRestaurant(Restaurant* restaurant) {
        restaurants.push_back(restaurant);
    }

    vector<Restaurant*> searchByLocation(string loc) {
        vector<Restaurant*> results;
        transform(loc.begin(), loc.end(), loc.begin(), ::tolower);
        for(auto r : restaurants){
            string rl = r->getLocation();
            transform(rl.begin(), rl.end(), rl.begin(), ::tolower);
            if(rl == loc){
                results.push_back(r);
            }
        }
        return results;
    }
};

RestaurantManager* RestaurantManager::instance = nullptr; 

#endif // RESTAURANT_MANAGER_H