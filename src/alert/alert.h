#ifndef ALERT_H
#define ALERT_H

#include <stdio.h>
#include <stdlib.h>

int check_thresholds(float temperature, float humidity);
int send_alert(const char *message);
int send_email_alert(const char *recipient_email, const char *subject, const char *body);

#endif
