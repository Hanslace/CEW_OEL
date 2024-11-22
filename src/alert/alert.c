#include "alert.h"

#define TEMP_THRESHOLD 25.0
#define HUMIDITY_THRESHOLD 50.0
#define PRESSURE_THRESHOLD 1015.0
#define VISIBILITY_THRESHOLD 5000.0
#define WIND_SPEED_THRESHOLD 10.0

#define SMTP_SERVER "smtp://smtp.gmail.com:587"

int check_thresholds(float temperature, float humidity, float pressure, float visibility, float wind_speed) {
    char alert_message[256];

    if (temperature > TEMP_THRESHOLD) {
        snprintf(alert_message, sizeof(alert_message), "Temperature Alert! Current Temp: %.2f°C exceeds threshold of %.2f°C.", temperature, TEMP_THRESHOLD);
        send_alert(alert_message);
        send_email_alert(getenv("RECIPIENT_EMAIL"), "Temperature Alert", alert_message);
    }
    if (humidity > HUMIDITY_THRESHOLD) {
        snprintf(alert_message, sizeof(alert_message), "Humidity Alert! Current Humidity: %.2f%% exceeds threshold of %.2f%%.", humidity, HUMIDITY_THRESHOLD);
        send_alert(alert_message);
        send_email_alert(getenv("RECIPIENT_EMAIL"), "Humidity Alert", alert_message);
    }
    if (pressure < PRESSURE_THRESHOLD) {
        snprintf(alert_message, sizeof(alert_message), "Pressure Alert! Current Pressure: %.2f hPa is below the threshold of %.2f hPa.", pressure, PRESSURE_THRESHOLD);
        send_alert(alert_message);
        send_email_alert(getenv("RECIPIENT_EMAIL"), "Pressure Alert", alert_message);
    }
    if (visibility < VISIBILITY_THRESHOLD) {
        snprintf(alert_message, sizeof(alert_message), "Visibility Alert! Current Visibility: %.2f m is below the threshold of %.2f m.", visibility, VISIBILITY_THRESHOLD);
        send_alert(alert_message);
        send_email_alert(getenv("RECIPIENT_EMAIL"), "Visibility Alert", alert_message);
    }
    if (wind_speed > WIND_SPEED_THRESHOLD) {
        snprintf(alert_message, sizeof(alert_message), "Wind Speed Alert! Current Wind Speed: %.2f m/s exceeds threshold of %.2f m/s.", wind_speed, WIND_SPEED_THRESHOLD);
        send_alert(alert_message);
        send_email_alert(getenv("RECIPIENT_EMAIL"), "Wind Speed Alert", alert_message);
    }

    return 0;
}

int send_alert(const char *message) {
    char command[512];

    // Pop-up Notification
    snprintf(command, sizeof(command), "zenity --warning --text='%s' --title='Environmental Alert'", message);
    system(command);

    return 0;
}

int send_email_alert(const char *recipient_email, const char *subject, const char *body) {
    char command[1024];

    // Fetch sender email and password from environment
    const char *sender_email = getenv("SENDER_EMAIL");
    const char *sender_password = getenv("SENDER_PASSWORD");

    if (!sender_email || !sender_password) {
        fprintf(stderr, "Error: Missing email credentials in environment variables\n");
        return 1;
    }

    // Construct the curl command
    snprintf(command, sizeof(command),
             "curl --url '%s' --ssl-reqd "
             "--mail-from '%s' "
             "--mail-rcpt '%s' "
             "--user '%s:%s' "
             "--upload-file - <<EOF\n"
             "From: <%s>\n"
             "To: <%s>\n"
             "Subject: %s\n\n"
             "%s\n"
             "EOF",
             SMTP_SERVER, sender_email, recipient_email,
             sender_email, sender_password, sender_email, recipient_email, subject, body);

    // Execute the command
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Failed to send email alert. Command returned %d\n", status);
        return 1;
    }

    printf("Email alert sent successfully to %s\n", recipient_email);
    return 0;
}
