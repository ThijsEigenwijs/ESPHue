/**
 * ESP8266 Hue Controller
 *
 * Created by Tom van Rooij 
 * Industrial Design Student, Eindhoven University of Technology
 * www.tomvrooij.nl
 *
 * Last edit: 25-04-2016
**/

#ifndef ESPHue_h
#define ESPHue_h

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include <ArduinoJson.h>

class ESPHue {
	public:
	
		//Setup functions
		ESPHue(WiFiClient& _hc, const char* _bridge_ip, const char* _bridge_user );
		ESPHue(WiFiClient& _hc);
		void setDebug( const bool _debug );
		void setupIP( const char* _bridge_ip );
		void setupUser( const char* _bridge_user );
		void setup( const char* _bridge_ip, const char* _bridge_user );
		
		// Color Settings
		void xy( const float _x, const float _y );
		void xySet( const uint8_t _light_id, const float _x, const float _y );
		
		/*
		//RGB Settings
		void rgb( const uint8_t _r, const uint8_t _g, const uint8_t _b );
		void rgbSet( const uint8_t _light_id, const uint8_t _r, const uint8_t _g, const uint8_t _b );
		*/
		
		//On functions
		void on();
		void on( const bool _on );
		void onSet( const uint8_t _light_id, const bool _on );
		void onSet( const uint8_t _light_id );
		bool onGet();
		
		//Off functions
		void off();
		void offSet( const uint8_t _light_id );
		
		//Hue functions
		void hue( const uint16_t _hue );
		void hueSet( const uint8_t _light_id, const uint16_t _hue );
		uint16_t hueGet();
		
		//Color Temperature 
		void ct( const uint16_t _ct );
		void ctSet( const uint8_t _light_id, const uint16_t _ct );
		uint16_t ctGet();
		
		//Brightness
		void bri( const uint8_t _bri );
		void briSet( const uint8_t _light_id, const uint8_t _bri );
		uint8_t briGet();
		
		//Saturation
		void sat( const uint8_t _sat );
		void satSet( const uint8_t _light_id, const uint8_t _sat );
		uint8_t satGet();
		
		// ??
		void HSB( const uint16_t _hue, const uint8_t _sat, const uint8_t _bri );
		void HSBSet( const uint8_t _light_id, const uint16_t _hue, const uint8_t _sat, const uint8_t _bri );
		void HSBOnSet( const uint8_t _light_id, const uint16_t _hue, const uint8_t _sat, const uint8_t _bri, const bool _on );
		
		// ??
		void CB( const uint16_t _ct, const uint8_t _bri );
		void CBSet( const uint8_t _light_id, const uint16_t _ct, const uint8_t _bri );
		
		// Set a single light
		bool set( const uint8_t _light_id );
		//bool set( const uint8_t _light_id, const String _command );
		
		// Set a group
		// (All lights are group 0)
		bool setGroup( const uint8_t _group_id );
		//bool setGroup( const uint8_t _group_id, const String _command );
		
		// Get commands
		String getLightInfo(int _light_id); //Need to change to private
		bool getLightState(int _light_id);
		int getLightBri(int _light_id);
		int getLightSat(int _light_id);
		int getLightHue(int _light_id);
		int getLightCt(int _light_id);
		String getLightName(int _light_id);
		int getLightX(int _light_id);
		int getLightY(int _light_id);
				
		
		String getGroupInfo(int _group_id); //Need to change to private
		bool getGroupState(int _group_id); 
		
	private:
		bool debug = false; //Debug off by default
		char* bridge_ip;
		char* bridge_user;
		uint16_t hue_v, ct_v;
		uint8_t sat_v, bri_v;
		float x, y;
		bool on_v, on_f, hue_f, ct_f, sat_f, bri_f, xy_f, sent_f;
		void resetFlags();
		WiFiClient* hc;

		struct _lightInfo {
			int on;
			int bri;
			int hue;
			int sat;
			int x;
			int y;
			int ct;
		};

		_lightInfo lightInfo;
};
#endif