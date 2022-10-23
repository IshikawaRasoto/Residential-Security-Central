/*
    RFIDCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "defineESPCentral.hpp"
#include "RFIDCentral.hpp"
#include <MFRC522.h>


#define SD_CLK_PIN       25
#define SD_MOSI_PIN      26
#define SD_MISO_PIN      32
#define SD_CS_PIN        33

//using namespace ext;

byte sdaPins[] = {SDA_ENTRADA, SDA_SAIDA};

MFRC522 mfrc522[2];

SPIClass SPI2 (HSPI);

File dados;

String IDtag = ""; 

void configuraRFID_SD(){

    pinMode(SD_CS_PIN, OUTPUT);
    pinMode(CS, OUTPUT); //SD
    pinMode(SDA_ENTRADA, OUTPUT); //RFID
    pinMode(SDA_SAIDA, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode (14, INPUT); //BOTAO TEMPORARIO

    SPI2.begin(SD_CLK_PIN , SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

    SD.begin(SD_CS_PIN, SPI2);

    
    for (uint8_t reader = 0; reader < 2; reader++) {
      mfrc522[reader].PCD_Init(sdaPins[reader], RESET);
      Serial.print(F("Reader "));
      Serial.print(reader);
      Serial.print(F(": "));
      mfrc522[reader].PCD_DumpVersionToSerial();
      }

}

void verificaRFID(){

    if(digitalRead(14) == HIGH){
      Serial.println("Aproxime a TAG para cadastrar");
      cadastraTAG();
    }else{

      if (mfrc522[0].PICC_IsNewCardPresent() && mfrc522[0].PICC_ReadCardSerial()) { //ENTRADA
            Serial.println("RFID 0 encontrado");
            salvaTAG(0);
            Serial.println(IDtag);
            verificaTAG("simples");
            IDtag = "";
            mfrc522[0].PICC_HaltA();
      } 
    
      if (mfrc522[1].PICC_IsNewCardPresent() && mfrc522[1].PICC_ReadCardSerial()) { //SAIDA
            Serial.println("RFID 1 encontrado");
            salvaTAG(1);
            Serial.println(IDtag);
            verificaTAG("simples");
            IDtag = "";
            mfrc522[1].PICC_HaltA();
      } 
    }
    delay(100);
}

void verificaTXT(){

    
    if (!SD.begin(SD_CS_PIN)){
        while (!SD.begin(SD_CS_PIN)){
            Serial.println ("Erro ao inicializar cartão SD"); //ALTERAR
            delay(1000);
        } 
    }

    else{
        if(!SD.exists("/Dados.txt")){
            Serial.println("Dados.txt está sendo criado");
            dados = SD.open ("/Dados.txt", FILE_WRITE);
            dados.close();
        }
    }

}




void verificaTAG(String tipo){

    String IDtagTemp = "";
    dados = SD.open("/Dados.txt");


    if (tipo == "simples"){
        if (dados){
            while (dados.available()){
                char letra = dados.read();

                if(isPrintable(letra)){
                    IDtagTemp.concat (letra);
                }

                else if(letra == '\n'){
                    if(IDtag ==  IDtagTemp){ //revisar
                        acessoLiberado();
                        dados.close();
                        return;
                    }
                    else{
                        IDtagTemp = "";
                    }
                }
            }
            acessoNegado();
            dados.close();
        }
    }
    
    else if (tipo == "complexa"){
        if (dados){
            while (dados.available()){
                char letra = dados.read();

                if(isPrintable(letra)){
                    IDtagTemp.concat (letra);
                }

                else if(letra == '\n'){
                    if(IDtag ==  IDtagTemp){ 
                        jaCadastrado();
                        dados.close();
                        return;
                    }
                    else{
                        IDtagTemp = "";
                    }
                }
            }
            //cadastraTAG_SD();ERRADO
        }
    }
    
}

void salvaTAG (const int x){
    for (byte i = 0; i < mfrc522[x].uid.size; i++) {        
        IDtag.concat(String(mfrc522[x].uid.uidByte[i], HEX));
    } 
}

void cadastraTAG(){

    while (!mfrc522[1].PICC_IsNewCardPresent() || !mfrc522[1].PICC_ReadCardSerial()){ //APENAS PODE CADASTRAR UMA TAG DENTRO DE CASA
        delay(200);
        Serial.println("Esperando...");
        //COMPLETAR COM FUNCOES
    }
    salvaTAG(1);
    //verificaTAG("complexa");
    cadastraTAG_SD();

}

void cadastraTAG_SD(){
    dados = SD.open("/Dados.txt", FILE_APPEND);
    if (dados){
      Serial.print("Cadastrando TAG: ");
      Serial.println(IDtag);
      dados.println(IDtag);
      dados.close(); 
    }
    IDtag = "";
}

void jaCadastrado(){
    
}

void acessoLiberado(){
    Serial.println("Cartão encontrado");
}
void acessoNegado(){
    Serial.println("Cartão não encontrado");
}
