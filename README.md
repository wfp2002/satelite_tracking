# satelite_tracking
Projeto para tracking de satelite usando linux, arduino e motor de passo.

Ferramentas Necessarias:

Sistema Utilizado Lubuntu

Hardware
- Motor de Passo Modelo: 28BYJ-48
- Arduino

Software
- GPredict
sudo apt-get install gpredict

- Hamlib (Para comunicacao com Arduino usando rotctld)
sudo apt-get install libhamlib-utils

Comando para executar no terminal apos hamlib instalado
rotctld -m 202 -s 19200 -v -r /dev/ttyACM0

-m 202 (Utiliza easycommII)
-s 19200 (Serial baudrate 19200)
-v Verbose Mode
-r /dev/ttyACM0 (USB qe o arduino esta conectado, pode variar como ttyUSB0 etc...)

----------------------------------------------------------------------------------

Configurando GPredict

1) Acesse o Gpredict e entre no menu (EDIT depois UPDATE TLE) para Atualizar a lista de satelites.
2) Depois Menu (EDIT - PREFERENCIAS - INTERFACES - ROTATORS) e inserir um novo com os dados abaixo:
Name: easycomm2
Host: localhost
Port: 4533
AZ type: 0 - 180 - 360
Min AZ: 0 - Max AZ: 360
Min EL: 0 - Max EL: 90

----------------------------------------------------------------------------------

Sequencia de execucao:

1) Faca o Upload do Codigo para o Arduino
2) Rode o Comando no terminal: rotctld -m 202 -s 19200 -v -r /dev/ttyACM0
3) Abra o GPredict selecione o Satelite e depois na tela principal no icone "setinha pra baixo" no canto superior direito, escolha Antenna Control, Clique em Track depois em Engage.

Pronto os motores ja vao seguir o satelite selecionado.

