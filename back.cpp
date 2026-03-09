#include "crow_all.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

int numeroSecreto;
int tentativas = 0;
const int MAX_TENTATIVAS = 7;

void novoJogo() {
    numeroSecreto = rand() % 100 + 1;
    tentativas = 0;
    cout << "\n================================" << endl;
    cout << "TRAPACA: O numero e: " << numeroSecreto << endl;
    cout << "================================\n" << endl;
}

int main() {
    crow::SimpleApp app;
    srand(time(0));
    novoJogo();

    // Rota Principal
    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load("index.html");
        crow::mustache::context ctx;
        ctx["resultado"] = "BOA SORTE, JOGADOR!";
        ctx["finalizado"] = false;
        return page.render(ctx);
    });

    // Rota do Jogo
    CROW_ROUTE(app, "/jogar").methods(crow::HTTPMethod::Post)([](const crow::request& req){
        auto page = crow::mustache::load("index.html");
        crow::mustache::context ctx;
        
        // Pega o palpite enviado pelo formulário
        auto x = crow::query_string("?" + req.body); 
        string mensagem = "";
        bool acabou = false;

        if (x.get("palpite") != nullptr) {
            int palpite = stoi(x.get("palpite"));
            tentativas++;

            if (palpite == numeroSecreto) {
                mensagem = "VITÓRIA! O NÚMERO ERA " + to_string(numeroSecreto) + ".\nACERTOU EM " + to_string(tentativas) + " TENTATIVAS!";
                acabou = true;
                novoJogo(); // Reinicia para a próxima
            } 
            else if (tentativas >= MAX_TENTATIVAS) {
                mensagem = "GAME OVER! ESGOTOU AS TENTATIVAS.\nO NÚMERO ERA " + to_string(numeroSecreto);
                acabou = true;
                novoJogo();
            }
            else if (palpite > numeroSecreto) {
                mensagem = "TENTATIVA " + to_string(tentativas) + ": MUITO ALTO! ↓";
            } 
            else {
                mensagem = "TENTATIVA " + to_string(tentativas) + ": MUITO BAIXO! ↑";
            }
        }

        ctx["resultado"] = mensagem;
        ctx["finalizado"] = acabou;
        return page.render(ctx);
    });

    // Rota do CSS (Mantenha igual)
    CROW_ROUTE(app, "/style.css")([](){
        crow::response res;
        res.set_static_file_info("static/style.css");
        res.set_header("Content-Type", "text/css");
        return res;
    });

    app.port(18080).multithreaded().run();
}