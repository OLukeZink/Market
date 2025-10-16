// main.cpp - minimal console shop using Product class

#include "../include/functions.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <optional>

using namespace std;

// Implement seedProducts and checkCompatibility here so only two files are used.
vector<Product> seedProducts() {
	// Atomic: return a catalog with exactly one product to keep things simple.
	vector<Product> catalog;
	catalog.push_back(Product(101, Category::CPU, "AMD Ryzen 5 5600X", 199.99, 10).socket("AM4").tdp(65));
	return catalog;
}

CompatibilityResult checkCompatibility(const vector<Product>& selection) {
	CompatibilityResult res{true, {}};
	optional<Product> cpu, mobo, ram, gpu, psu, pcCase;
	for (const auto &p : selection) {
		switch (p.category()) {
			case Category::CPU: if (!cpu) cpu = p; break;
			case Category::Motherboard: if (!mobo) mobo = p; break;
			case Category::RAM: if (!ram) ram = p; break;
			case Category::GPU: if (!gpu) gpu = p; break;
			case Category::PSU: if (!psu) psu = p; break;
			case Category::Case_: if (!pcCase) pcCase = p; break;
			default: break;
		}
	}

	if (cpu && mobo) {
		if (!cpu->socket().has_value() || !mobo->socket().has_value()) {
			res.ok = false; res.messages.push_back("Missing socket info on CPU or motherboard.");
		} else if (cpu->socket().value() != mobo->socket().value()) {
			res.ok = false; res.messages.push_back("CPU socket mismatch.");
		}
	}

	if (ram && mobo) {
		if (!ram->ramType().has_value() || !mobo->ramType().has_value()) {
			res.ok = false; res.messages.push_back("Missing RAM type info.");
		} else if (ram->ramType().value() != mobo->ramType().value()) {
			res.ok = false; res.messages.push_back("RAM generation mismatch.");
		}
	}

	if (gpu && pcCase) {
		if (gpu->lengthMm().has_value() && pcCase->formFactor().has_value()) {
			if (pcCase->formFactor().value() == "mATX" && gpu->lengthMm().value() > 280) {
				res.ok = false; res.messages.push_back("GPU may be too long for this mATX case.");
			}
		}
	}

	if (psu) {
		int required = 0;
		if (cpu && cpu->tdp().has_value()) required += cpu->tdp().value();
		if (gpu && gpu->tdp().has_value()) required += gpu->tdp().value();
		required = static_cast<int>(required * 1.3);
		if (psu->psuWattage().has_value() && psu->psuWattage().value() < required) {
			res.ok = false; res.messages.push_back("PSU wattage may be insufficient.");
		}
	}

	if (res.ok) res.messages.push_back("Basic compatibility checks passed.");
	return res;
}

// Minimal console UI
int main() {
	auto catalog = seedProducts();
	vector<Product> cart;

	while (true) {
		cout << "\nSimple Market - Menu:\n";
		cout << "1) List products\n";
		cout << "2) Add product to cart (enter product number)\n";
		cout << "3) View cart\n";
		cout << "4) Check compatibility\n";
		cout << "5) Clear cart\n";
		cout << "0) Exit\n";
		cout << "Choose: ";
		int choice;
		if (!(cin >> choice)) break;

		if (choice == 0) break;
		if (choice == 1) {
			cout << "\nProducts:\n";
			for (size_t i = 0; i < catalog.size(); ++i) {
				cout << i+1 << ": " << catalog[i].name() << " ($" << catalog[i].price() << ")\n";
			}
		} else if (choice == 2) {
			cout << "Enter product number to add: ";
			int n; cin >> n;
			if (n >= 1 && (size_t)n <= catalog.size()) {
				cart.push_back(catalog[n-1]);
				cout << "Added: " << catalog[n-1].name() << "\n";
			} else cout << "Invalid product number\n";
		} else if (choice == 3) {
			cout << "\nCart items:\n";
			for (size_t i = 0; i < cart.size(); ++i) cout << i+1 << ": " << cart[i].name() << "\n";
		} else if (choice == 4) {
			auto res = checkCompatibility(cart);
			cout << "\nCompatibility result: " << (res.ok ? "OK" : "ISSUES") << "\n";
			for (auto &m : res.messages) cout << " - " << m << "\n";
		} else if (choice == 5) {
			cart.clear(); cout << "Cart cleared.\n";
		} else {
			cout << "Unknown choice\n";
		}
	}

	cout << "Goodbye\n";
	return 0;
}

