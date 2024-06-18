//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       _              //
//               _    _       _      _        _     _   _   _    _   _   _        _   _  _   _          //
//           |  | |  |_| |\| |_| |\ |_|   |\ |_|   |_| |_| | |  |   |_| |_| |\/| |_| |  |_| | |   /|    //    
//         |_|  |_|  |\  | | | | |/ | |   |/ | |   |   |\  |_|  |_| |\  | | |  | | | |_ | | |_|   _|_   //
//                                                                                       /              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*   Programa básico para controle da placa durante a Jornada da Programação 1
*   Permite o controle das entradas e saídas digitais, entradas analógicas, display LCD e teclado. 
*   Cada biblioteca pode ser consultada na respectiva pasta em componentes
*   Existem algumas imagens e outros documentos na pasta Recursos
*   O código principal pode ser escrito a partir da linha 86
*/

// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ioplaca.h"   // Controles das Entradas e Saídas digitais e do teclado
#include "lcdvia595.h" // Controles do Display LCD
#include "hcf_adc.h"   // Controles do ADC
#include "MP_hcf.h"   // Controles do ADC

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------

int controle = 0;
int num1 = 0; //primeiro valor
int coluna = 0;
int resultado = 0;
char operador;
char tecla;
char mostra[40];
int senha;
int contagem;
int tentativas = 0;
int i;
char seguranca;
// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------

static const char *TAG = "Placa";
static uint8_t entradas, saidas = 0; //variáveis de controle de entradas e saídas
uint32_t adc; 
uint32_t valor; 

// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------



// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void abrir ()    //Programa para abrir o cofre
{
     lcd595_write (1, 0, "Abrindo Cofre");
     for (int i = 0; i < 1800 ; i = valor)
     {
        hcf_adc_ler (&valor);
        rotacionar_DRV (1,10, saidas);
        seguranca ++;
        if (seguranca > 15)break;
     }
     seguranca = 0;
}

void fechar ()   //Programa para fechar o cofre
{
     hcf_adc_ler (&valor);
     for (int i = valor; i > 400 ; i = valor)
     {
        hcf_adc_ler (&valor);
        rotacionar_DRV (0,10, saidas);
        seguranca ++;
        if (seguranca > 15)break;
     }
     seguranca = 0;
}


void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////   Programa principal


    // a seguir, apenas informações de console, aquelas notas verdes no início da execução
    ESP_LOGI(TAG, "Iniciando...");
    ESP_LOGI(TAG, "Versão do IDF: %s", esp_get_idf_version());

    /////////////////////////////////////////////////////////////////////////////////////   Inicializações de periféricos (manter assim)
    
    // inicializar os IOs e teclado da placa
    ioinit();      
    entradas = io_le_escreve(saidas); // Limpa as saídas e lê o estado das entradas

    // inicializar o display LCD 
    lcd595_init();
    lcd595_write(1,1,"   COFRE  ");
    lcd595_write(2,1," DA SARA");
    
    // Inicializar o componente de leitura de entrada analógica
    esp_err_t init_result = hcf_adc_iniciar();
    if (init_result != ESP_OK) {
        ESP_LOGE("MAIN", "Erro ao inicializar o componente ADC personalizado");
    }

    //delay inicial
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
    lcd595_clear();

    /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados


    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1)                                                                                                                         
    {                                                                                                                                 
        //_______________________________________________________________________________________________________________________________________________________ //
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu código aqui!!! //
                                                                                                                                                                  
        tecla = le_teclado();     //Ler o teclado 
        entradas= io_le_escreve(saidas);  
        hcf_adc_ler (&adc);  //Ler o ADC


        if (tecla >='0'&& tecla <= '9')   // Fazer os números aparecer no display    _ _ _ _ 
        {  
          controle = controle + 1;   // Passar para o número do lado
          num1 =  num1 * 10 + tecla - '0';   // Passar para o número do lado
        }  

       if ( controle==0)  //Digitar a senha
        {              
          lcd595_write(1,1," DIGITE A SENHA");   ///Aparecer no display 
          lcd595_write(2,0,"[ ] [ ] [ ] [ ]");    //Aparecer no display 
          vTaskDelay(50/ portTICK_PERIOD_MS);   //Tempo de Delay
        }
        if ( controle==1)  //Digitar a senha
        {  
          lcd595_write(1,1," DIGITE A SENHA");  ///Aparecer no display  
          lcd595_write(2,0,"[x] [ ] [ ] [ ]");    ///Aparecer no display 
          vTaskDelay(50/ portTICK_PERIOD_MS);   //Tempo de Delay
        }

            if ( controle==2) 
        {
          lcd595_write(2,0,"[x] [x] [ ] [ ]");   ///Aparecer no display 
          vTaskDelay(50/ portTICK_PERIOD_MS);   //Tempo de Delay
        }

            if ( controle==3)
        {
          lcd595_write(2,0,"[x] [x] [x] [ ]"); ///Aparecer no display 
          vTaskDelay(50/ portTICK_PERIOD_MS);   //Tempo de Delay
        }

            if ( controle==4)
        {   
          lcd595_write(2,0,"[x] [x] [x] [x]");    ///Aparecer no display 
          vTaskDelay(50/ portTICK_PERIOD_MS);   //Tempo de Delay
        }
          

        if (controle == 4) //Abrir cofre   
        { 
           if(num1 == 1234)
      
          { 
            lcd595_write(1,1, "SENHA CORRETA" );   ///Aparecer no display 
            lcd595_clear();   //Limpar para não ter sobreposição
            abrir (); //Abrir cofre
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 10s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS);   
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 9s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS);   
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 8s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS);   
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 7s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS); 
            lcd595_clear();   //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 6s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS); 
            lcd595_clear();  //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 5s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS);  
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 4s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS);  
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 3s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS);  
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 2s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS); 
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (2, 0, "Fechando em 1s");///Aparecer no display 
            vTaskDelay(1000/ portTICK_PERIOD_MS); 
            lcd595_clear(); //Limpar para não ter sobreposição
            lcd595_write (1, 0, "Fechando Cofre");///Aparecer no display 
            fechar();   //Fechar motor 
            lcd595_clear();  //Limpar para não ter sobreposição
            num1=0;  //Retomar ao zero
            controle=0; //Retomar ao zero
          }


          else  //Senha errada 
        { 
          lcd595_clear(); //Limpar para não ter sobreposição
          lcd595_write(1,1, "ACESSO NEGADO" );  ///Aparecer no display 
          lcd595_write(2,0, "TENTE NOVAMENTE" );   ///Aparecer no display  
          vTaskDelay(1000/ portTICK_PERIOD_MS);   //Tempo de Delay
          lcd595_clear(); //Limpar para não ter sobreposição
          num1=0;   //Retomar ao zero
          controle=0;  //Retomar ao zero
          tentativas = tentativas + 1; //Ir para a proxima tentaiva 

          if (tentativas == 1)
          {
          lcd595_write(1,0, "RESTAM 2 TENT" );  ///Aparecer no display 
          lcd595_write(2,0, "TENTE NOVAMENTE" );  ///Aparecer no display 
          vTaskDelay(1000/ portTICK_PERIOD_MS);   //Tempo de Delay
          lcd595_clear();   //Limpar para não ter sobreposição
          }

              if (tentativas == 2)
          {
          lcd595_write(1,0, "RESTAM 1 TENT" );  ///Aparecer no display 
          lcd595_write(2,0, "TENTE NOVAMENTE" );  ///Aparecer no display 
          vTaskDelay(1000/ portTICK_PERIOD_MS);   //Tempo de Delay
          lcd595_clear();   //Limpar para não ter sobreposição
          }

              if (tentativas == 3)
          {
          lcd595_write(1,0, "ACESSO BLOQUEADO" );  ///Aparecer no display 
          lcd595_write(2,0, "AGUARDE 30 SEGS" );  ///Aparecer no display 

            for (int i =30; i >0; i-- ) //Repetir uma ação
            {
              sprintf (mostra,"AGUARDE %d SEGS",i ); //Mostrar no display
              lcd595_write(2,0, mostra);  //Mostrar no display
              vTaskDelay(1000/ portTICK_PERIOD_MS);   //Tempo de Delay
              
            }
          num1=0;  //Retomar ao zero
          controle=0; //Retomar ao zero
          lcd595_clear();   //Limpar para não ter sobreposição
            
          }
           lcd595_clear();   //Limpar para não ter sobreposição
        }  
        }
      
         
         
        
     
    
            
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        vTaskDelay(10 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
  }
    
    // caso erro no programa, desliga o módulo ADC
    hcf_adc_limpar();

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
}
    



