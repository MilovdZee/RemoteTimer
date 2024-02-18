int str_to_time(char * input) {
  int hour, minute;
  sscanf(input, "%2d:%2d", &hour, &minute);
  return (60 * hour + minute) * 60; // second of day
}

int get_second_of_day() {
  time_t now = time(nullptr); // seconds since epoch
  struct tm *timeinfo = localtime (&now); // setup timeinfo -> tm_hour, timeinfo -> tm_min, timeinfo -> tm_sec
  return (60 * timeinfo->tm_hour + timeinfo->tm_min) * 60; // second of day
}