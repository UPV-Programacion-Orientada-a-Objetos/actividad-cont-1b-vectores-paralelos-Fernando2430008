#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

int main() {

    const int TAMANIO_MAXIMO = 100;
    int codigos_producto[TAMANIO_MAXIMO] = {0};
    std::string nombres_producto[TAMANIO_MAXIMO];
    int cantidades_producto[TAMANIO_MAXIMO] = {0};
    float precios_producto[TAMANIO_MAXIMO] = {0.0f};
    std::string ubicaciones_producto[TAMANIO_MAXIMO];
    
    int total_productos = 0;
    int opcion;

    std::ifstream archivo_entrada("inventario.txt");
    if (!archivo_entrada.is_open()) {
        std::cerr << "Advertencia: No se pudo abrir 'inventario.txt'. Se iniciara con un inventario vacio." << std::endl;
    } else {
        std::string linea, campo;
        std::getline(archivo_entrada, linea);

        while (std::getline(archivo_entrada, linea) && total_productos < TAMANIO_MAXIMO) {
            std::stringstream ss(linea);
            
            std::getline(ss, campo, ',');
            codigos_producto[total_productos] = std::stoi(campo);
            std::getline(ss, campo, ',');
            nombres_producto[total_productos] = campo;
            std::getline(ss, campo, ',');
            cantidades_producto[total_productos] = std::stoi(campo);
            std::getline(ss, campo, ',');
            precios_producto[total_productos] = std::stof(campo);
            std::getline(ss, campo, ',');
            ubicaciones_producto[total_productos] = campo;

            total_productos++;
        }
        archivo_entrada.close();
        std::cout << "Inventario cargado exitosamente. Total de productos: " << total_productos << std::endl;
    }

    std::cout << "\n== Sistema Avanzado de Inventario 'El Martillo' ==" << std::endl;
    do {
        std::cout << "\n-- Menu Principal --" << std::endl;
        std::cout << "1. Registrar nuevo producto" << std::endl;
        std::cout << "2. Actualizar stock por ubicacion" << std::endl;
        std::cout << "3. Generar reporte de bajo stock" << std::endl;
        std::cout << "4. Encontrar producto mas barato" << std::endl;
        std::cout << "0. Guardar y Salir" << std::endl;

        std::cout << "Ingrese su opcion: ";
        std::cin >> opcion;

        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Ingrese su opcion nuevamente: ";
            std::cin >> opcion;
        }

        switch (opcion) {
            case 1: { 
                std::cout << "\n- Registro de Nuevo Producto -" << std::endl;
                if (total_productos >= TAMANIO_MAXIMO) {
                    std::cout << "El inventario esta lleno. No se pueden agregar mas productos." << std::endl;
                    break;
                }

                int nuevoCodigo;
                bool codigoRepetido;

                do {
                    std::cout << "Ingrese el codigo del producto: ";
                    std::cin >> nuevoCodigo;
                    while (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Entrada invalida. Ingrese el codigo nuevamente: ";
                        std::cin >> nuevoCodigo;
                    }
                    
                    codigoRepetido = false;
                    for (int i = 0; i < total_productos; ++i) {
                        if (codigos_producto[i] == nuevoCodigo) {
                            codigoRepetido = true;
                            break;
                        }
                    }

                    if (codigoRepetido) {
                        std::cout << "Error: El codigo " << nuevoCodigo << " ya existe. Ingrese un codigo diferente." << std::endl;
                    }
                } while (codigoRepetido);

                codigos_producto[total_productos] = nuevoCodigo;

                std::cout << "Ingrese el nombre del producto: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, nombres_producto[total_productos]);

                std::cout << "Ingrese la cantidad en stock: ";
                std::cin >> cantidades_producto[total_productos];

                std::cout << "Ingrese el precio unitario: ";
                std::cin >> precios_producto[total_productos];

                std::cout << "Ingrese la ubicacion en almacen (ej. A-01): ";
                std::cin >> ubicaciones_producto[total_productos];

                total_productos++;
                std::cout << "\nProducto registrado correctamente." << std::endl;
                break;
            }
            case 2: {
                std::cout << "\n- Actualizacion de Stock por Ubicacion -" << std::endl;
                std::string ubicacionBuscada;
                std::cout << "Ingrese la ubicacion del producto a actualizar (ej. A-01): ";
                std::cin >> ubicacionBuscada;
                
                int indiceEncontrado = -1;
                for (int i = 0; i < total_productos; ++i) {
                    if (ubicaciones_producto[i] == ubicacionBuscada) {
                        indiceEncontrado = i;
                        break;
                    }
                }
                
                if (indiceEncontrado != -1) {
                    std::cout << "Producto encontrado: " << nombres_producto[indiceEncontrado] << std::endl;
                    std::cout << "Cantidad actual en stock: " << cantidades_producto[indiceEncontrado] << std::endl;
                    
                    int nuevaCantidad;
                    std::cout << "Ingrese la nueva cantidad en stock: ";
                    std::cin >> nuevaCantidad;
                    
                    cantidades_producto[indiceEncontrado] = nuevaCantidad;
                    std::cout << "Stock actualizado correctamente." << std::endl;
                } else {
                    std::cout << "Error: No se encontro ningun producto en la ubicacion '" << ubicacionBuscada << "'." << std::endl;
                }
                break;
            }
            case 3: {
                const int UMBRAL_BAJO_STOCK = 10;
                std::cout << "\n- Reporte de Productos con Bajo Stock (Menos de " << UMBRAL_BAJO_STOCK << " unidades) -" << std::endl;
                std::cout << "------------------------------------------------------------" << std::endl;
                std::cout << "Codigo\t| Nombre\t\t| Stock\t| Ubicacion" << std::endl;
                std::cout << "------------------------------------------------------------" << std::endl;

                bool encontrados = false;
                for (int i = 0; i < total_productos; ++i) {
                    if (cantidades_producto[i] < UMBRAL_BAJO_STOCK) {
                        std::cout << codigos_producto[i] << "\t| " << nombres_producto[i] << "\t| " << cantidades_producto[i] << "\t| " << ubicaciones_producto[i] << std::endl;
                        encontrados = true;
                    }
                }
                if (!encontrados) {
                    std::cout << "No hay productos con bajo stock." << std::endl;
                }
                std::cout << "------------------------------------------------------------" << std::endl;
                break;
            }
            case 4: { 
                std::cout << "\n- Busqueda del Producto Mas Barato -" << std::endl;
                if (total_productos == 0) {
                    std::cout << "El inventario esta vacio." << std::endl;
                    break;
                }
                int indiceBarato = 0;
                for (int i = 1; i < total_productos; ++i) {
                    if (precios_producto[i] < precios_producto[indiceBarato]) {
                        indiceBarato = i;
                    }
                }
                std::cout << "El producto mas barato es: " << nombres_producto[indiceBarato] << std::endl;
                std::cout << "Precio: $" << precios_producto[indiceBarato] << std::endl;
                break;
            }
            case 0: { 
                std::cout << "\nGuardando cambios en el inventario..." << std::endl;
                std::ofstream archivo_salida("inventario.txt");
                if (!archivo_salida.is_open()) {
                    std::cerr << "Error critico: No se pudo abrir 'inventario.txt' para guardar los datos." << std::endl;
                } else {
                    archivo_salida << "Código,Nombre,Cantidad,Precio,Ubicación\n";
                    for (int i = 0; i < total_productos; ++i) {
                        archivo_salida << codigos_producto[i] << ","
                                    << nombres_producto[i] << ","
                                    << cantidades_producto[i] << ","
                                    << precios_producto[i] << ","
                                    << ubicaciones_producto[i] << "\n";
                    }
                    archivo_salida.close();
                    std::cout << "Datos guardados. Saliendo del programa." << std::endl;
                }
                break;
            }
            default: {
                std::cout << "\nOpcion no valida. Por favor, intente de nuevo." << std::endl;
                break;
            }
        }
    } while (opcion != 0);

    return 0;
}
