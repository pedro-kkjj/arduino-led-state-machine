 
int led2 = 4; //vermelho
int led1 = 5; //verde
int botao = 6;
int fase = 0; //fase
int modo = 0; //modo
int botAnterior = 0; //estado botao
int botAtual = 0;

int led1Ativo = 0; //contador led
int led2Ativo = 0;            

unsigned long tempoLed1 = 0;
unsigned long tempoLed2 = 0;

bool estadoLed1 = LOW;
bool estadoLed2 = LOW;

int a = 0; // contadores de tempo
int b = 0;
int c = 0;


void desligarLed(int led)
{ 
digitalWrite(led,LOW);
}//desliga led

void ligarLed(int led)
{ 
digitalWrite(led,HIGH);
}//liga led

void ligarSeAtivo(int led, int ativo)
{
  if (ativo == 1)
  {
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }
}//liga o led se ele estiver ativo. (desliga se não)

void piscarSeAtivo(int led, int ativo, unsigned long &tempo, bool &estado, int intervalo)
{
  if (ativo == 1)
  { 
    if (millis() - tempo >= intervalo)
    {
      tempo = millis();
      estado = !estado;
      digitalWrite(led, estado);
    }
  }
else
  { 
  desligarLed(led);
  } 
}//verifica se o led está ativo e, se estiver, pisca em certo intervalo (desliga se não)

void definirLedsAtivos()
{
   if(fase == 1) //fase 1
 {
   led1Ativo = 1;
   led2Ativo = 0;
    
   
 }else if(fase == 2) //fase 2
 {
   led1Ativo = 0;
   led2Ativo = 1;

 }else if(fase == 3) //fase 3
 {
   led1Ativo = 1;
   led2Ativo = 1;

 }else
 {
   led1Ativo = 0;
   led2Ativo = 0;
   
   desligarLed(led1);
   desligarLed(led2); 
 }
}//define qual led estará ligado a depender da fase

void detectarBotao()
{
 if(botAnterior == LOW && botAtual == HIGH)
  {
  a = millis();
   
  tempoLed1 = millis(); 
  tempoLed2 = millis();  

  estadoLed1 = LOW;
  estadoLed2 = LOW; 
   
  }  //assim que o botao é clicado, reseta algumas variaveis e começa a contar o tempo
  if(botAnterior == HIGH && botAtual == LOW)
 {
        
  b = millis();  
  c = b - a;   
    
    if(c < 500){    
    fase++;     
    if(fase > 3 )fase = 0;
      
   Serial.print("-CLIQUE CURTO- ");  
      
   }else if(c > 500){  
   Serial.print("-CLIQUE LONGO- ");      
   modo++;
   if(modo > 3 )modo = 0;      
  } 
 }//qnd o botao é solto, ve qnd tempo ficou apertado e decide o tipo de ação
    
 botAnterior = botAtual;
}

void mostrarInfo()
{ 
 Serial.print("Fase: ");
 Serial.print(fase);
 Serial.print(". Modo: ");
 Serial.println(modo);
}//função para mostrar informações
  
void executarModo()
{
  if(modo == 1) //modo 1 
 { 
  piscarSeAtivo(led1, led1Ativo, tempoLed1, estadoLed1, 500);
  piscarSeAtivo(led2, led2Ativo, tempoLed2, estadoLed2, 500);
 }else if (modo == 2)//modo 2
 {
  piscarSeAtivo(led1, led1Ativo, tempoLed1, estadoLed1, 200);
  piscarSeAtivo(led2, led2Ativo, tempoLed2, estadoLed2, 200);
 }else if(modo == 3)//modo 3
   
{
  ligarSeAtivo(led1, led1Ativo);
  ligarSeAtivo(led2, led2Ativo);
  
 }else{
      desligarLed(led1);
      desligarLed(led2);
    } 
}  //função do modo

void setup()
{
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
 pinMode(botao, INPUT);
 Serial.begin(9600);
}


void loop()
{
 botAtual = digitalRead(botao);
  
 mostrarInfo();
 detectarBotao();
 definirLedsAtivos();
 executarModo(); 

} //final
