#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <Arduino.h>

#define LED_PIN 22
#define WATER_SENSOR_PIN 32
#define BUZZER_PIN 23

const char* ssid = "<Your_SSID>";
const char* password = "<Your_Password>";

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587

#define EMAIL_ADDRESS "<Your_Email_Address>"
#define EMAIL_PASSWORD "<Your_Email_Password>"
#define RECIPIENT_EMAIL "<Recipient_Email_Address>"
#define SUBJECT "Flooding Alert: Flood Detected!"
#define BODY "Attention: A Flood has been detected! Please check the sensor immediately."

SMTPSession smtp;

// Function to send the email
void sendEmail() {
    /* Configure SMTP session */
    Session_Config config;
    config.server.host_name = SMTP_HOST;
    config.server.port = SMTP_PORT;
    config.login.email = EMAIL_ADDRESS;
    config.login.password = EMAIL_PASSWORD;
    config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    config.time.gmt_offset = 3;
    config.time.day_light_offset = 0;

    /* Configure email message */
    SMTP_Message message;
    message.sender.name = F("ESP32 Flood Alert");
    message.sender.email = EMAIL_ADDRESS;
    message.subject = SUBJECT;
    message.addRecipient(F("Recipient"), RECIPIENT_EMAIL);
    message.text.content = BODY;
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    smtp.debug(1);
    smtp.callback(smtpCallback);

    if (!smtp.connect(&config)) {
        Serial.printf("Connection error: %s\n", smtp.errorReason().c_str());
        return;
    }

    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.printf("Error sending email: %s\n", smtp.errorReason().c_str());
    } else {
        Serial.println("Flooding alert email sent successfully!");
    }

    smtp.sendingResult.clear();
}

// SMTP callback to handle the result of the email attempt
void smtpCallback(SMTP_Status status) {
    Serial.println(status.info());
    if (status.success()) {
        Serial.println("Email sent successfully.");
    } else {
        Serial.println("Email failed to send.");
    }
}

// Function to read the water sensor and trigger actions
void readWaterSensor(void *pvParameters) {
    while (true) {
        int waterStatus = digitalRead(WATER_SENSOR_PIN);
        Serial.println(waterStatus);

        if (waterStatus == HIGH) {
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(BUZZER_PIN, HIGH);
            Serial.println("Water detected!");
            sendEmail();
            vTaskDelay(30000 / portTICK_PERIOD_MS); // Prevent spamming email alerts (use vTaskDelay)
        } else {
            digitalWrite(LED_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            Serial.println("No water detected!");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay to reduce unnecessary checks
    }
}

// Wi-Fi connection handling in a separate task
void handleWiFi(void *pvParameters) {
    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Reconnecting to Wi-Fi...");
            WiFi.reconnect();
            vTaskDelay(5000 / portTICK_PERIOD_MS); // Wait for 5 seconds before retrying
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Check every second
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(WATER_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Connected to Wi-Fi");

    // Enable network reconnection
    MailClient.networkReconnect(true);

    // Create tasks for reading water sensor and handling Wi-Fi
    xTaskCreate(readWaterSensor, "ReadWaterSensor", 8192, NULL, 1, NULL); // Reduced stack size for water sensor task
    xTaskCreate(handleWiFi, "HandleWiFi", 4096, NULL, 1, NULL); // Task for handling Wi-Fi reconnection
}

void loop() {
    // Nothing needed here, as RTOS tasks handle the functionality
}
