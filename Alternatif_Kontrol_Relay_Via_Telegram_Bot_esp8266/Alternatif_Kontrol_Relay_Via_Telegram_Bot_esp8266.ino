#include <WiFi.h>
#include <CTBot.h>
CTBot myBot;

#define Output_Relay1   15 // Pin D1
#define Output_Relay2   2 // Pin D2
#define Output_Relay3   5// Pin D3
#define Output_Relay4   26 // Pin D4

 int ON = HIGH, OFF = LOW;//UBAH JIKA MENGGUNAKAN RELAY AKTIF LOW

  String ssid  = "Safirudin 2";                                 
  String pass  = "safirudin54321";                                
               
  String token = "";   // token
  const int id = ; 	// id
   
  // Untuk Setting Perintah Pada Telegram Ganti Nama Dalam Tanda Kutip Sesuai Keinginan Anda 
     const char* Perintah_On_1 = "/NyalakanLampuTeras";
     const char* Perintah_On_2 = "/NyalakanLampuJalan";
     const char* Perintah_On_3 = "/NyalakanLampuBelakang";
     const char* Perintah_On_4 = "/NyalakanMesinPompa";
     const char* Perintah_On_5 = "/Mau_Lebih_Banyak_Relay_Order_YUK";

     const char* Perintah_Off_1 = "/MatikanLampuTeras";
     const char* Perintah_Off_2 = "/MatikanLampuJalan";
     const char* Perintah_Off_3 = "/MatikanLampuBelakang";
     const char* Perintah_Off_4 = "/MatikanMesinPompa";
     const char* Perintah_Off_5 = "/Mau_Lebih_Banyak_Relay_Order_YUK";

void setup() {
Serial.begin(115200);                               // initialize Serial
	Serial.println("Setup Telegram Bot...");
	myBot.wifiConnect(ssid, pass);                   // Mengkoneksikan Ke WiFi
	myBot.setTelegramToken(token);                  // Mengkonfigurasikan Token

    if (myBot.testConnection())   
	      Serial.println("\nKoneksi Ke BOT OK"); 
	      else Serial.println("\nTidak Terkoneksi Ke BOT");   // Chek koneksi internet Telegram 
	
	      // Pengaturan Fungsi Pin GPIOs Menjadi Output
	         pinMode(Output_Relay1, OUTPUT);         
           pinMode(Output_Relay2, OUTPUT);      
           pinMode(Output_Relay3, OUTPUT);       
           pinMode(Output_Relay4, OUTPUT);      
           
	         digitalWrite(Output_Relay1, OFF);   
           digitalWrite(Output_Relay2, OFF);   
           digitalWrite(Output_Relay3, OFF);      
           digitalWrite(Output_Relay4, OFF);      
      Serial.println("Perintah Telegram Siap Di Fungsikan...");
}

void loop() {
TBMessage msg;

if (myBot.getNewMessage(msg)) {                                                               //Membaca Pesan Masuk...
		if (msg.text.equalsIgnoreCase(Perintah_On_1)) {                                             // Membaca Perintah Menyalakan Jika Ada
			  digitalWrite(Output_Relay1, ON);     
			  myBot.sendMessage(msg.sender.id, "Lampu Teras Sudah Di Nyalakan");}      
		else if (msg.text.equalsIgnoreCase(Perintah_Off_1)) {                                       // Membaca Perintah Menyalakan Jika Ada
			      digitalWrite(Output_Relay1, OFF);                                                   
			        myBot.sendMessage(msg.sender.id, "Lampu Teras Sudah Di Matikan");}          
    else if (msg.text.equalsIgnoreCase(Perintah_On_2)) {                                        // Membaca Perintah Menyalakan Jika Ada
            digitalWrite(Output_Relay2, ON);                                                    // Nyalakan Output_Relay2
              myBot.sendMessage(msg.sender.id, "Lampu Jalan Sudah Di Nyalakan");}      // Kirim Balasan Lampu Berhasil Di Nyalakan
    else if (msg.text.equalsIgnoreCase(Perintah_Off_2)) {                                       // Membaca Perintah Menyalakan Jika Ada
            digitalWrite(Output_Relay2, OFF);                                                   // Matikan Output_Relay2
              myBot.sendMessage(msg.sender.id, "Lampu Jalan Sudah Di Matikan");}                // Kirim Balasan Lampu Berhasil Di Nyalakan          
    else if (msg.text.equalsIgnoreCase(Perintah_On_3)) {                                        // Membaca Perintah Menyalakan Jika Ada
            digitalWrite(Output_Relay3, ON);                                                    // Nyalakan Output_Relay3
              myBot.sendMessage(msg.sender.id, "Lampu Belakang Sudah Di Nyalakan");}   // Kirim Balasan Lampu Berhasil Di Nyalakan
    else if (msg.text.equalsIgnoreCase(Perintah_Off_3)) {                                       // Membaca Perintah Menyalakan Jika Ada
            digitalWrite(Output_Relay3, OFF);                                                   // Matikan Output_Relay3
              myBot.sendMessage(msg.sender.id, "Lampu Belakang Sudah Di Matikan");}             // Kirim Balasan Lampu Berhasil Di Nyalakan    
    else if (msg.text.equalsIgnoreCase(Perintah_On_4)) {                                        // Membaca Perintah Menyalakan Jika Ada
            digitalWrite(Output_Relay4, ON);                                                    // Nyalakan Output_Relay4
              myBot.sendMessage(msg.sender.id, "Mesin Pompa Sudah Di Nyalakan");}      // Kirim Balasan Lampu Berhasil Di Nyalakan
    else if (msg.text.equalsIgnoreCase(Perintah_Off_4)) {                                       // Membaca Perintah Menyalakan Jika Ada
            digitalWrite(Output_Relay4, OFF);                                                   // Matikan Output_Relay4
              myBot.sendMessage(msg.sender.id, "Mesin Pompa Sudah Di Matikan");}                // Kirim Balasan Lampu Berhasil Di Nyalakan      
    else if (msg.text.equalsIgnoreCase("/StatusRelay")) {                                       // Membaca Perintah
             myBot.sendMessage(msg.sender.id, 
             "\nThank You...");}// Kirim Balasan                 
		else {                                                                                      // Jika Pesan Tidak Ada Dalam Daftar Perintah Maka Akan Kirimkan Hint
	   String Balasan;
			Balasan = (String)"Selamat Datang " //+ msg.sender.username + (String)
			          "\nControl IOT \nTidak Ditemukan Perintah."
			          "\n\nCoba Gunakan Perintah Berikut:"
                "\nLampu Teras :\n\t"
                  + String(Perintah_On_1)+"\n\t"
                  + String(Perintah_Off_1)+"\n"
                "\nLampu Jalan :\n\t"
                  + String(Perintah_On_2)+"\n"
                  + String(Perintah_Off_2)+"\n"
                "\nLampu Belakang :\n\t"
                  + String(Perintah_On_3)+"\n\t"
                  + String(Perintah_Off_3)+"\n"
                "\nMesin Pompa :\n\t"
                  + String(Perintah_On_4)+"\n\t"
                  + String(Perintah_Off_4)+"\n"
                "\nChekStatus Relay :\n\t"
                  "/StatusRelay\n"
                
			      
			;myBot.sendMessage(msg.sender.id, Balasan);             // and send it
		}
	}

 Serial.println(".");
	// delay(500);
}
