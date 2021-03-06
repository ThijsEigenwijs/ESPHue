/**
 * ESP8266 Hue Controller Library
 *
 * Created by Tom van Rooij
 * Industrial Design Student, Eindhoven University of Technology
 * www.tomvrooij.nl
 *
 * Last edit: 25-04-2016
**/

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESPHue.h"

ESPHue::ESPHue(WiFiClient& _hc, const char* _bridge_ip, const char* _bridge_user) {
	bridge_ip = (char*)_bridge_ip;
	bridge_user = (char*)_bridge_user;
	hc = &_hc;
	resetFlags();
	return;
}

ESPHue::ESPHue(WiFiClient& _hc) {
	//Do nothing, everything will be set later;
	resetFlags();
	hc = &_hc;
	return;
}

void ESPHue::setDebug(const bool _debug) {
	debug = _debug;
}

void ESPHue::setupIP(const char* _bridge_ip) {
	bridge_ip = (char*)_bridge_ip;
}

void ESPHue::setupUser(const char* _bridge_user) {
	bridge_user = (char*)_bridge_user;
}

void ESPHue::setup(const char* _bridge_ip, const char* _bridge_user) {
	bridge_ip = (char*)_bridge_ip;
	bridge_user = (char*)_bridge_user;
}

////////// ON/OFF STATE //////////
void ESPHue::on(const bool _on) {
	if (sent_f) resetFlags();
	on_v = _on;
	on_f = true;
}

void ESPHue::on() {
	if (sent_f) resetFlags();
	on_v = true;
	on_f = true;
}

void ESPHue::onSet(const uint8_t _light_id, const bool _on) {
	resetFlags();
	on(_on);
	set(_light_id);
}

void ESPHue::onSet(const uint8_t _light_id) {
	resetFlags();
	on();
	set(_light_id);
}

bool ESPHue::onGet() {
	return on_v;
}

void ESPHue::off() {
	if (sent_f) resetFlags();
	on_v = false;
	on_f = true;
}

void ESPHue::offSet(const uint8_t _light_id) {
	resetFlags();
	off();
	set(_light_id);
}
///////////////////////////////

////////// HUE COLOR //////////
void ESPHue::hue(const uint16_t _hue) {
	if (sent_f) resetFlags();
	hue_v = _hue;
	hue_f = true;
}

void ESPHue::hueSet(const uint8_t _light_id, const uint16_t _hue) {
	resetFlags();
	hue(_hue);
	set(_light_id);
}

uint16_t ESPHue::hueGet() {
	return hue_v;
}
////////////////////////////////

////////// COLOR TEMPERATURE //////////
void ESPHue::ct(const uint16_t _ct) {
	if (sent_f) resetFlags();
	ct_v = _ct;
	ct_f = true;
}

void ESPHue::ctSet(const uint8_t _light_id, const uint16_t _ct) {
	resetFlags();
	ct(_ct);
	set(_light_id);
}

uint16_t ESPHue::ctGet() {
	return ct_v;
}
////////////////////////////////

////////// SATURATION //////////
void ESPHue::sat(const uint8_t _sat) {
	if (sent_f) resetFlags();
	sat_v = _sat;
	sat_f = true;
}

void ESPHue::satSet(const uint8_t _light_id, const uint8_t _sat) {
	resetFlags();
	sat(_sat);
	set(_light_id);
}

uint8_t ESPHue::satGet() {
	return sat_v;
}
///////////////////////////////

////////// BRIGHTNESS //////////
void ESPHue::bri(const uint8_t _bri) {
	if (sent_f) resetFlags();
	bri_v = _bri;
	bri_f = true;
}

void ESPHue::briSet(const uint8_t _light_id, const uint8_t _bri) {
	resetFlags();
	bri(_bri);
	set(_light_id);
}

uint8_t ESPHue::briGet() {
	return bri_v;
}
//////////////////////////////

/*
//////////// RGB //////////////
void ESPHue::rgb( const uint8_t _r, const uint8_t _g, const uint8_t _b );
	if( sent_f ) resetFlags();

	xy_f = true;
}

void ESPHue::rgbSet( const uint8_t _light_id, const uint8_t _r, const uint8_t _g, const uint8_t _b );
	resetFlags();
	xy( _x, _y );
	set( _light_id );
}

//convert rgb to xy
public static List<Double> getRGBtoXY(Color c) {
	// For the hue bulb the corners of the triangle are:
	// -Red: 0.675, 0.322
	// -Green: 0.4091, 0.518
	// -Blue: 0.167, 0.04
	double[] normalizedToOne = new double[3];
	float cred, cgreen, cblue;
	cred = c.getRed();
	cgreen = c.getGreen();
	cblue = c.getBlue();
	normalizedToOne[0] = (cred / 255);
	normalizedToOne[1] = (cgreen / 255);
	normalizedToOne[2] = (cblue / 255);
	float red, green, blue;

	// Make red more vivid
	if (normalizedToOne[0] > 0.04045) {
		red = (float) Math.pow(
				(normalizedToOne[0] + 0.055) / (1.0 + 0.055), 2.4);
	} else {
		red = (float) (normalizedToOne[0] / 12.92);
	}

	// Make green more vivid
	if (normalizedToOne[1] > 0.04045) {
		green = (float) Math.pow((normalizedToOne[1] + 0.055)
				/ (1.0 + 0.055), 2.4);
	} else {
		green = (float) (normalizedToOne[1] / 12.92);
	}

	// Make blue more vivid
	if (normalizedToOne[2] > 0.04045) {
		blue = (float) Math.pow((normalizedToOne[2] + 0.055)
				/ (1.0 + 0.055), 2.4);
	} else {
		blue = (float) (normalizedToOne[2] / 12.92);
	}

	float X = (float) (red * 0.649926 + green * 0.103455 + blue * 0.197109);
	float Y = (float) (red * 0.234327 + green * 0.743075 + blue * 0.022598);
	float Z = (float) (red * 0.0000000 + green * 0.053077 + blue * 1.035763);

	float x = X / (X + Y + Z);
	float y = Y / (X + Y + Z);

	double[] xy = new double[2];
	xy[0] = x;
	xy[1] = y;
	List<Double> xyAsList = Doubles.asList(xy);
	return xyAsList;
}
//////////////////////////////
*/

//////////// XY //////////////
void ESPHue::xy(const float _x, const float _y) {
	if (sent_f) resetFlags();
	x = _x;
	y = _y;
	xy_f = true;
}

void ESPHue::xySet(const uint8_t _light_id, const float _x, const float _y) {
	resetFlags();
	xy(_x, _y);
	set(_light_id);
}
//////////////////////////////

////////// COMBINED FUNCTIONS //////////
void ESPHue::HSB(const uint16_t _hue, const uint8_t _sat, const uint8_t _bri) {
	resetFlags();
	hue(_hue);
	sat(_sat);
	bri(_bri);
}

void ESPHue::CB(const uint16_t _ct, const uint8_t _bri) {
	resetFlags();
	ct(_ct);
	bri(_bri);
}

void ESPHue::HSBSet(const uint8_t _light_id, const uint16_t _hue, const uint8_t _sat, const uint8_t _bri) {
	resetFlags();
	hue(_hue);
	sat(_sat);
	bri(_bri);
	set(_light_id);
}
void ESPHue::HSBOnSet(const uint8_t _light_id, const uint16_t _hue, const uint8_t _sat, const uint8_t _bri, const bool _on) {
	resetFlags();
	on(_on);
	hue(_hue);
	sat(_sat);
	bri(_bri);
	set(_light_id);
}

void ESPHue::CBSet(const uint8_t _light_id, const uint16_t _ct, const uint8_t _bri) {
	resetFlags();
	ct(_ct);
	bri(_bri);
	set(_light_id);
}

///////////////////////////////////////

////////// SENDING FUNCTIONS //////////
//  sending format json:
//	{\"on\":false,\"ct\":153,\"sat\":0,\"bri\":0,\"hue\":0,\"xy\":[0.675,0.322]}
//    false|true	 153-500	   0-255	 0-255	   0-65535		http://www.developers.meethue.com/documentation/core-concepts

bool ESPHue::set(const uint8_t _light_id) {
	String json = "{";
	bool entry = false;
	if (on_f) {
		json += "\"on\":";
		if (on_v) json += "true";
		else json += "false";
		entry = true;
	}
	if (xy_f) {
		if (entry) json += ",";
		json += "\"xy\":[";
		json += x;
		json += ",";
		json += y;
		json += "]";
		resetFlags();
	}
	if (hue_f) {
		if (entry) json += ",";
		json += "\"hue\":";
		json += hue_v;
		entry = true;
	}
	if (ct_f) {
		if (entry) json += ",";
		json += "\"ct\":";
		json += ct_v;
		entry = true;
	}
	if (sat_f) {
		if (entry) json += ",";
		json += "\"sat\":";
		json += sat_v;
		entry = true;
	}
	if (bri_f) {
		if (entry) json += ",";
		json += "\"bri\":";
		json += bri_v;
		entry = true;
	}
	json += "}";

	if (json != "") {
		WiFiClient hc; //hueclient
		if (hc.connect((const char*)bridge_ip, 80)) {
			String message = "PUT /api/";
			message += bridge_user;
			message += "/lights/";
			message += _light_id;
			message += "/state HTTP/1.1\r\nHost:";
			message += bridge_ip;
			message += "\r\nConnection:close";
			message += "\r\nContent-Type:application/json";
			message += "\r\nContent-Length:";
			message += json.length();
			message += "\r\n\r\n";
			message += json;
			message += "\r\n\r\n";
			hc.print(message);
			if (debug) Serial.println(message);
			sent_f = true;
		}
		hc.stop();
	}
	return true;
}


bool ESPHue::setGroup(const uint8_t _group_id) {
	String json = "{";
	bool entry = false;
	if (on_f) {
		json += "\"on\":";
		if (on_v) json += "true";
		else json += "false";
		entry = true;
	}
	if (xy_f) {
		if (entry) json += ",";
		json += "\"xy\":[";
		json += x;
		json += ",";
		json += y;
		json += "]";
		resetFlags();
	}
	if (hue_f) {
		if (entry) json += ",";
		json += "\"hue\":";
		json += hue_v;
		entry = true;
	}
	if (ct_f) {
		if (entry) json += ",";
		json += "\"ct\":";
		json += ct_v;
		entry = true;
	}
	if (sat_f) {
		if (entry) json += ",";
		json += "\"sat\":";
		json += sat_v;
		entry = true;
	}
	if (bri_f) {
		if (entry) json += ",";
		json += "\"bri\":";
		json += bri_v;
		entry = true;
	}
	json += "}";
	bool succes = false;

	if (json != "") {
		WiFiClient hc; //hueclient
		if (hc.connect((const char*)bridge_ip, 80)) {
			String message = "PUT /api/";
			message += bridge_user;
			message += "/groups/";
			message += _group_id;
			message += "/state HTTP/1.1\r\nHost:";
			message += bridge_ip;
			message += "\r\nConnection:close";
			message += "\r\nContent-Type:application/json";
			message += "\r\nContent-Length:";
			message += json.length();
			message += "\r\n\r\n";
			message += json;
			message += "\r\n\r\n";
			hc.print(message);
			if (debug) Serial.println(message);
			sent_f = true;
			succes = true;
		}
		hc.stop();
		if (debug) {
			if (succes) Serial.print("Sent to IP: \t");
			else Serial.print("Failed to send to IP: \t");
			Serial.println(bridge_ip);
			Serial.print("With username: \t");
			Serial.println(bridge_user);
		}
	}
	if (succes) return true;
	else return false;
}


void ESPHue::resetFlags() {
	on_f = false;
	hue_f = false;
	ct_f = false;
	sat_f = false;
	bri_f = false;
	xy_f = false;
	sent_f = false;
}
/////////////////////////////////////////////
/*
String ESPHue::getJson( ) {
	if( amount > 0 ) {
		if( amount == 1 ) {
			String json = "{";
			json += command[0];
			json += "}";
			return json;
		}
		else {
			String json = "{";
			json += command[0];
			for( uint8_t i=1; i<amount; i++) {
				json += ",";
				json += command[i];
			}
			json += "}";
			return json;
		}
	}
}

String ESPHue::get( ) {
	if( amount > 0 ) {
		String output = "";
		for( uint8_t i=1; i<amount; i++) {
			output += command[i];
			output += " ";
		}
		return output;
	}
}


HueSetting::HueSetting( uint8_t _amount ) {
	amount = _amount;
}
*/

//////////////////// Get Commands //////////////////

String ESPHue::getLightInfo(int _light_id) {

	Serial.println("Don't worry, this can take a while to process everything!");

	String importantLine;

	WiFiClient client;
	if (!client.connect(bridge_ip, 80)) {
		Serial.println("Connection Failed");
		return "";
	}

	String message = String("/api/") + bridge_user + "/lights/" + _light_id;
	client.print("GET " + message + " HTTP/1.1\r\nHost: " + bridge_ip + "\r\nConnection: close\r\n\r\n");
	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println("Timeout");
			client.stop();
			return "";
		}
	}

	while (client.available()) {
		
		String line = client.readString();
		importantLine = line.substring(line.indexOf("{"));
		String data = importantLine.substring(importantLine.indexOf("{") + 9, importantLine.indexOf("}") + 1);
		Serial.println(data);

		const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(11) + 200;
		DynamicJsonBuffer jsonBuffer(bufferSize);

		JsonObject& root = jsonBuffer.parseObject(data);

		bool on = root["on"]; // true
		int bri = root["bri"]; // 144
		int hue = root["hue"]; // 7676
		int sat = root["sat"]; // 199
		const char* effect = root["effect"]; // "none"

		float xy0 = root["xy"][0]; // 0.5016
		float xy1 = root["xy"][1]; // 0.4151

		int ct = root["ct"]; // 443
		const char* colormode = root["colormode"]; // "xy"
		bool reachable = root["reachable"]; // true

		if (!root.success()) {
			Serial.println("parseObject() failed");
			return "";
		}

		Serial.print("On : "); Serial.println(on);
		Serial.print("Bri: "); Serial.println(bri);
		Serial.print("Hue: "); Serial.println(hue);
		Serial.print("Sat: "); Serial.println(sat);
		Serial.print("X  : "); Serial.println(xy0);
		Serial.print("Y  : "); Serial.println(xy1);
		Serial.print("CT : "); Serial.println(ct);

		lightInfo.on = on;
		lightInfo.bri = bri;
		lightInfo.hue = hue;
		lightInfo.sat = sat;
		lightInfo.x = xy0;
		lightInfo.y = xy1;
		lightInfo.ct = ct;

	}

	Serial.println();
	Serial.println("closing connection");
	return importantLine;
}

bool ESPHue::getLightState(int _light_id) {
	return lightInfo.on;
}

int ESPHue::getLightBri(int _light_id) {
	return lightInfo.bri;
}
int ESPHue::getLightSat(int _light_id) {
	return lightInfo.sat;
}
int ESPHue::getLightHue(int _light_id) {
	return lightInfo.hue;
}
int ESPHue::getLightCt(int _light_id) {
	return lightInfo.ct;
}
String ESPHue::getLightName(int _light_id) {

}
int ESPHue::getLightX(int _light_id) {
	return lightInfo.x;
}
int ESPHue::getLightY(int _light_id) {
	return lightInfo.y;
}