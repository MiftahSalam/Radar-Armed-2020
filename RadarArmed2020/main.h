#ifndef MAIN_H
#define MAIN_H

#include "simrad_global.h"

RadarState state = RADAR_OFF;
ReportFilter filter;
ReportAlign align;
ReportScanSignal scanSignal;
RadarSettings radar_settings;
ARPASettings arpa_settings;
GZSettings gz_settings;
TrailSettings trail_settings;

double currentOwnShipLat;
double currentOwnShipLon;
double radarHeading;
double currentHeading;
bool headingUp;

bool enable_mti;
int mti_value;

#endif // MAIN_H
