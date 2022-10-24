/*
    ESPCAM.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

volatile bool modoAP;
volatile bool statusComunicacao;

volatile char statusWiFi;

#include "defineESPCAM.hpp"
#include "ESPCam.hpp"
#include "EEPROMCam.hpp"

void configuracao(){
    pinMode(TRANCA, OUTPUT);
    serialConfig();
}

void executar(){
    //verificaDadosWiFi();
    operacoes();
    //verificaConexao();
    //verificaModoAP();
    testeESPComunicacao();
}

void operacoes(){
}

void serialConfig(){
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_CENTRAL, TX_CENTRAL);
}

void inicializarVariaveis(){
    modoAP = false;
    statusComunicacao = false;
}

void verificaConexao(){
    //testeConexao(&statusWiFi);
}

void verificaModoAP(){
    if(modoAP)
        //limparMemoria();
    if(testeCredenciais())
        return;
    modoAP = true;
    //while(enviaFormulario());
}

void testeESPComunicacao(){
  
    while(!Serial1.available()){
        
    }
    String mensagem = Serial1.readString();
    Serial.println(mensagem);
    if(mensagem == "TesteComunicacao"){
        Serial1.print(mensagem);
    }
    else if(mensagem == "abrir"){
        digitalWrite (TRANCA, HIGH);
        delay(5000);
        digitalWrite (TRANCA, LOW);
        Serial1.print("resetRFID");
    }
    else if("modoAP"){
        modoAP = true;
    }
}
