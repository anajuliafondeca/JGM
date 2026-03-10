#include "crow_all.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

// Variáveis globais do jogo
int numeroSecreto;
int tentativas = 0;
const int MAX_TENTATIVAS = 7;

// Função para iniciar ou reiniciar o jogo
void novoJogo() {
    numeroSecreto = rand() % 100 + 1;
    tentativas = 0;
    cout << "\n================================" << endl;
    cout << "TRAPAÇA: O numero e: " << numeroSecreto << endl;
    cout << "================================\n" << endl;
}

int main() {
    crow::SimpleApp app;
    srand(time(0));
    novoJogo();

    // Rota para o CSS (Manual para evitar erros de versão do Crow)
    CROW_ROUTE(app, "/style.css")([](){
        crow::response res;
        res.set_static_file_info("static/style.css");
        res.set_header("Content-Type", "text/css");
        return res;
    });

    // Rota Principal (Carrega o HTML inicial)
    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load("index.html");
        crow::mustache::context ctx;
        ctx["resultado"] = "BOA SORTE, JOGADOR!";
        ctx["finalizado"] = false;
        return page.render(ctx);
    });

    // Rota de Processamento do Jogo (POST)
    CROW_ROUTE(app, "/jogar").methods(crow::HTTPMethod::Post)([](const crow::request& req){
        auto page = crow::mustache::load("index.html");
        crow::mustache::context ctx;
        
        // Lê os dados do formulário enviado pelo HTML
        crow::query_string x("?" + req.body); 
        string mensagem = "";
        bool acabou = false;

        if (x.get("palpite") != nullptr) {
            try {
                int palpite = stoi(x.get("palpite"));
                tentativas++;

                if (palpite == numeroSecreto) {
                    mensagem = "VITORIA! O NUMERO ERA " + to_string(numeroSecreto) + ".\nACERTOU EM " + to_string(tentativas) + " TENTATIVAS!";
                    acabou = true;
                    novoJogo(); // Reinicia o número secreto para a próxima partida
                } 
                else if (tentativas >= MAX_TENTATIVAS) {
                    mensagem = "GAME OVER! ESGOTOU AS TENTATIVAS.\nO NUMERO ERA " + to_string(numeroSecreto);
                    acabou = true;
                    novoJogo();
                }
                else if (palpite > numeroSecreto) {
                    mensagem = "TENTATIVA " + to_string(tentativas) + ": MUITO ALTO! ↓";
                } 
                else {
                    mensagem = "TENTATIVA " + to_string(tentativas) + ": MUITO BAIXO! ↑";
                }
            } catch (...) {
                mensagem = "ERRO: INSIRA UM NUMERO VALIDO!";
            }
        }

        ctx["resultado"] = mensagem;
        ctx["finalizado"] = acabou;
        return page.render(ctx);
    });

    // --- CONFIGURAÇÃO PARA O RENDER ---
    // O Render exige que o app rode na porta que ele define via variável de ambiente PORT
    uint16_t port = 18080;
    char* env_port = getenv("PORT");
    if (env_port) {
        port = static_cast<uint16_t>(stoi(env_port));
    }

    cout << "Servidor Arcade Math rodando na porta: " << port << endl;
    
    // Roda o servidor
    app.port(port).multithreaded().run();
}