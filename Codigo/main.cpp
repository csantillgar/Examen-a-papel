#include <iostream>
#include <stdexcept>
#include <utility>

class Categoria {
private:
    int valor;

public:
    Categoria() : valor(0) {}

    int getValor() const {
        return valor;
    }

    void setValor(int nuevoValor) {
        if (nuevoValor >= 1 && nuevoValor <= 5) {
            valor = nuevoValor;
        }
    }
};

class Producto {
private:
    static int contadorIds;
    int id;
    std::string nombre;
    Categoria categoria;
    int numPuertos;
    int velocidad;
    bool averiado;

public:
    Producto(std::string  nombre, const Categoria& categoria)
            : id(++contadorIds), nombre(std::move(nombre)), categoria(categoria), numPuertos(0), velocidad(0), averiado(false) {}

    Producto(std::string  nombre, const Categoria& categoria, int numPuertos, int velocidad)
            : id(++contadorIds), nombre(std::move(nombre)), categoria(categoria), numPuertos(numPuertos), velocidad(velocidad), averiado(false) {}


    Producto(const Producto& other)
            : id(++contadorIds), nombre(other.nombre), categoria(other.categoria),
              numPuertos(other.numPuertos), velocidad(other.velocidad), averiado(other.averiado) {}

    void imprimir() const {
        std::cout << "ID: " << id << "\nNombre: " << nombre << "\nCategoría: " << categoria.getValor() << "\nAveriado: " << (averiado ? "Sí" : "No");
        if (categoria.getValor() >= 1 && categoria.getValor() <= 3) {
            std::cout << "\nNúmero de Puertos: " << numPuertos << "\nVelocidad: " << velocidad;
        }
        std::cout << "\n\n";
    }

    bool isAveriado() const {
        return averiado;
    }

    void setAveriado(bool estado) {
        averiado = estado;
    }

    void setCategoria(const Categoria& nuevaCategoria) {
        categoria = nuevaCategoria;
    }

    void setVelocidad(int nuevaVelocidad) {
        if (nuevaVelocidad % 10 == 0) {
            velocidad = nuevaVelocidad;
        }
    }

    // Agregar otros getters y setters según sea necesario
};

int Producto::contadorIds = 0;

class Laboratorio {
private:
    static const int MAX = 10;
    Producto* inventario[MAX];
    int cantidad;

public:
    Laboratorio() : cantidad(0) {
        for (int i = 0; i < MAX; ++i) {
            inventario[i] = nullptr;
        }
    }

    void addProducto(const Producto& producto) {
        if (cantidad < MAX) {
            inventario[cantidad++] = new Producto(producto);
        } else {
            throw std::runtime_error("No se puede agregar más productos. Inventario lleno.");
        }
    }

    bool delProducto(int id) {
        for (int i = 0; i < cantidad; ++i) {
            if (inventario[i]->isAveriado()) {
                delete inventario[i];
                inventario[i] = nullptr;
                return true;
            }
        }
        return false;
    }

    void imprimirInventario() const {
        std::cout << "Inventario del Laboratorio:\n";
        for (int i = 0; i < cantidad; ++i) {
            inventario[i]->imprimir();
        }
        std::cout << "Número de productos: " << cantidad << "\n";
    }

    void imprimirDefectuosos() const {
        std::cout << "Productos Defectuosos:\n";
        for (int i = 0; i < cantidad; ++i) {
            if (inventario[i]->isAveriado()) {
                inventario[i]->imprimir();
            }
        }
    }

    void imprimirEstadisticas() const {
        int buenos = 0;
        for (int i = 0; i < cantidad; ++i) {
            if (!inventario[i]->isAveriado()) {
                buenos++;
            }
        }
        float porcentajeDefectuosos = (float(cantidad - buenos) / cantidad) * 100;

        std::cout << "Estadísticas del Laboratorio:\n";
        std::cout << "Número total de productos: " << cantidad << "\n";
        std::cout << "Número de productos buenos: " << buenos << "\n";
        std::cout << "Porcentaje de productos defectuosos: " << porcentajeDefectuosos << "%\n";
    }

    ~Laboratorio() {
        for (int i = 0; i < cantidad; ++i) {
            delete inventario[i];
        }
    }

    // Agregar otros métodos según sea necesario
};

int main() {
    Laboratorio lab;

    while (true) {
        std::cout << "Menú:\n";
        std::cout << "1. Agregar Producto\n";
        std::cout << "2. Eliminar Producto\n";
        std::cout << "3. Mostrar Inventario\n";
        std::cout << "4. Salir del Programa\n";
        std::cout << "Seleccione una opción: ";

        int opcion;
        std::cin >> opcion;

        switch (opcion) {
            case 1: {
                std::string nombre;
                int categoriaValor;
                std::cout << "Ingrese el nombre del producto: ";
                std::cin >> nombre;
                std::cout << "Ingrese la categoría (1-ORDENADOR, 2-ROUTER, 3-SWITCH, 4-MATERIALFUNGIBLE, 5-OTROS): ";
                std::cin >> categoriaValor;

                Categoria categoria;
                categoria.setValor(categoriaValor);

                Producto nuevoProducto(nombre, categoria);

                int numPuertos, velocidad;
                if (categoriaValor >= 1 && categoriaValor <= 3) {
                    std::cout << "Ingrese el número de puertos: ";
                    std::cin >> numPuertos;
                    std::cout << "Ingrese la velocidad: ";
                    std::cin >> velocidad;
                    nuevoProducto = Producto(nombre, categoria, numPuertos, velocidad);
                }

                bool averiado;
                std::cout << "¿El producto está averiado? (1-Sí, 0-No): ";
                std::cin >> averiado;
                nuevoProducto.setAveriado(averiado);

                try {
                    lab.addProducto(nuevoProducto);
                    std::cout << "Producto agregado exitosamente.\n";
                    nuevoProducto.imprimir();
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }

                break;
            }
            case 2: {
                int id;
                std::cout << "Ingrese el ID del producto a eliminar: ";
                std::cin >> id;

                if (lab.delProducto(id)) {
                    std::cout << "Producto eliminado correctamente.\n";
                } else {
                    std::cout << "No se encontró el producto con el ID especificado.\n";
                }

                break;
            }
            case 3:
                lab.imprimirInventario();
                lab.imprimirEstadisticas();
                lab.imprimirDefectuosos();
                break;
            case 4:
                std::cout << "Saliendo del programa...\n";
                return 0;
            default:
                std::cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    }

    return 0;
}
