#pragma once

#include <string>
#include <vector>
#include <optional>

enum class Category { CPU, Motherboard, RAM, GPU, PSU, Case_, Storage, Unknown };

// Minimal Product class used by the console prototype. Keep it tiny.
class Product {
	int m_id;
	Category m_cat;
	std::string m_name;
	double m_price;
	int m_stock;

	std::optional<std::string> m_socket;
	std::optional<std::string> m_ramType;
	std::optional<int> m_tdp;
	std::optional<int> m_lengthMm;
	std::optional<int> m_psuWattage;
	std::optional<std::string> m_formFactor;

public:
	Product(int id=0, Category cat=Category::Unknown, const std::string &name="", double price=0.0, int stock=0)
		: m_id(id), m_cat(cat), m_name(name), m_price(price), m_stock(stock) {}

	// Fluent setters for optional fields (convenience for seeding)
	Product& socket(const std::string &s) { m_socket = s; return *this; }
	Product& ramType(const std::string &r) { m_ramType = r; return *this; }
	Product& tdp(int t) { m_tdp = t; return *this; }
	Product& length(int mm) { m_lengthMm = mm; return *this; }
	Product& psuWattage(int w) { m_psuWattage = w; return *this; }
	Product& formFactor(const std::string &f) { m_formFactor = f; return *this; }

	// Accessors used by main.cpp
	int id() const { return m_id; }
	Category category() const { return m_cat; }
	const std::string& name() const { return m_name; }
	double price() const { return m_price; }
	int stock() const { return m_stock; }

	const std::optional<std::string>& socket() const { return m_socket; }
	const std::optional<std::string>& ramType() const { return m_ramType; }
	const std::optional<int>& tdp() const { return m_tdp; }
	const std::optional<int>& lengthMm() const { return m_lengthMm; }
	const std::optional<int>& psuWattage() const { return m_psuWattage; }
	const std::optional<std::string>& formFactor() const { return m_formFactor; }
};

struct CompatibilityResult { bool ok; std::vector<std::string> messages; };

// Declarations (definitions are in main.cpp)
std::vector<Product> seedProducts();
CompatibilityResult checkCompatibility(const std::vector<Product>& selection);

// --- New: minimal Cpu class ---
// A focused class for CPU parts only. Keep it tiny: id, name, socket, tdp, price, stock.
class Cpu {
	int m_id;
	std::string m_name;
	std::string m_socket;
	int m_tdp; // watts
	double m_price;
	int m_stock;
public:
	Cpu(int id=0, const std::string &name = "", const std::string &socket = "", int tdp = 0, double price = 0.0, int stock = 0)
		: m_id(id), m_name(name), m_socket(socket), m_tdp(tdp), m_price(price), m_stock(stock) {}

	int id() const { return m_id; }
	const std::string& name() const { return m_name; }
	const std::string& socket() const { return m_socket; }
	int tdp() const { return m_tdp; }
	double price() const { return m_price; }
	int stock() const { return m_stock; }

	// Convenience: string for display
	std::string display() const {
		char buf[256];
		snprintf(buf, sizeof(buf), "%s (id:%d) socket:%s tdp:%dW $%.2f", m_name.c_str(), m_id, m_socket.c_str(), m_tdp, m_price);
		return std::string(buf);
	}
};

// Helper to seed a tiny CPU list (declaration only; implement if/when you want it used)
std::vector<Cpu> seedCpus();
