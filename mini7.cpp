#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <cmath>

class Product {
public:
    std::string name;
    std::string id;
    int quantity;
    int ordersSold; // New member variable to track number of orders sold
    double cost;
    double sell;

    Product(const std::string& name, const std::string& id, int quantity, double cost, double sell)
        : name(name), id(id), quantity(quantity), ordersSold(0), cost(cost), sell(sell) {}
};

class Inventory {
private:
    std::vector<Product> products;
    std::ofstream logFile;

public:
    Inventory() {
        logFile.open("inventory_log.txt", std::ios_base::app);
        if (!logFile.is_open()) {
            std::cerr << "Error opening log file." << std::endl;
        } else {
            logFile << "Inventory Log:\n";
        }
    }

    bool addProduct(const Product& newProduct) {
        products.push_back(newProduct);
        logFile << "Added product: " << newProduct.name << " (ID: " << newProduct.id << "), Quantity: " << newProduct.quantity << "\n";
        return true;
    }

    void updateProductQuantity(const std::string& id, int newQuantity) {
    bool found = false;
    for (auto& product : products) {
        if (product.id == id) {
            product.quantity += newQuantity; // Add the new quantity to the existing quantity
            logFile << "Updated product: " << product.name << " (ID: " << product.id << "), Quantity: " << product.quantity << "\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cerr << "Product not found." << std::endl;
    }
}


    void sellProduct(const std::string& id, int quantity) {
        bool found = false;
        for (auto& product : products) {
            if (product.id == id) {
                if (product.quantity >= quantity) {
                    product.quantity -= quantity;
                    product.ordersSold += quantity; // Update number of orders sold
                    std::cout << "Sold " << quantity << " units of " << product.name << ".\n";
                    found = true;
                } else {
                    std::cerr << "Insufficient quantity of " << product.name << " in stock.\n";
                }
                break;
            }
        }
        if (!found) {
            std::cerr << "Product not found." << std::endl;
        }
    }

    void displayInventory() const {
        std::cout << "Inventory:\n";
        for (const auto& product : products) {
            std::cout << "Name: " << product.name << ", ID: " << product.id << ", Quantity: " << product.quantity << ", Orders Sold: " << product.ordersSold << "\n";
        }
    }

    // Method to predict when a product is going to run out
    void predictStockout(const std::string& id) const {
        for (const auto& product : products) {
            if (product.id == id) {
                double averageSalesRate = static_cast<double>(product.ordersSold) / 30.0; // Assuming 30 days per month
                int daysToStockout = static_cast<int>(std::ceil(static_cast<double>(product.quantity) / averageSalesRate));
                if (daysToStockout > 0) {
                    std::cout << "Product " << product.name << " (ID: " << product.id << ") is expected to run out in approximately " << daysToStockout << " days.\n";
                } else {
                    std::cout << "Product " << product.name << " (ID: " << product.id << ") is already out of stock.\n";
                }
                return;
            }
        }
        std::cerr << "Product not found." << std::endl;
    }

    ~Inventory() {
        logFile.close();
    }
};

int main() {
    Inventory inventory;
    int choice;

    do {
        std::cout << "\n                                ==============================================\n";
        std::cout << "                                   Welcome to the Retail Management System\n";
        std::cout << "                                 ==============================================\n";
        std::cout << "What would you like to do?\n";
        std::cout << "1. Add Product\n";
        std::cout << "2. Update Product Quantity\n";
        std::cout << "3. Sell Product\n";
        std::cout << "4. Display Inventory\n";
        std::cout << "5. Predict Stockout\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {  
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
                std::string name, id;
                int quantity;
                double cost, sell;
                std::cout << "Enter product id: ";
                std::cin >> id;
                std::cout << "Enter product name: ";
                std::cin>> name;
                std::cout << "Enter product quantity: ";
                std::cin >> quantity;
                std::cout << "Enter product cost: ";
                std::cin >> cost;
                std::cout << "Enter product sell price: ";
                std::cin >> sell;
                if (inventory.addProduct(Product(name, id, quantity, cost, sell))) {
                    std::cout << "Product added successfully.\n";
                } else {
                    std::cerr << "Failed to add product.\n";
                }
                break;
            }
            case 2: {
                std::string id;
                int newQuantity;
                std::cout << "Enter product id of product to update: ";
                std::cin >> id;
                std::cout << "Enter new quantity: ";
                std::cin >> newQuantity;
                inventory.updateProductQuantity(id, newQuantity);
                break;
            }
            case 3: {
                std::string id;
                int quantity;
                std::cout << "Enter product id of product to sell: ";
                std::cin >> id;
                std::cout << "Enter quantity to sell: ";
                std::cin >> quantity;
                inventory.sellProduct(id, quantity);
                break;
            }
            case 4:
                inventory.displayInventory();
                break;
            case 5: {
                std::string id;
                std::cout << "Enter product id of product to predict stockout: ";
                std::cin >> id;
                inventory.predictStockout(id);
                break;
            }
            case 6:
                std::cout << "Are you sure you want to exit? (y/n): ";
                char confirm;
                std::cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    std::cout << "Exiting...\n";
                    break;
                } else {
                    std::cout << "Resuming...\n";
                    break;
                }
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
