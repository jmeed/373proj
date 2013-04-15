package com.example.swatch;

import java.util.Timer;
import java.util.TimerTask;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.widget.TextView;
import android.widget.Toast;

public class WeatherActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_weather);
		
		// Check if zip code saved
		if (Utility.mUserZip == null)
		{
			Toast.makeText(WeatherActivity.this, "Zip Code is invalid.  Please save zipcode and retry.", Toast.LENGTH_SHORT).show();
			finish();
			return;
		}
		
		TextView weatherTextView = (TextView) findViewById(R.id.weather_string);
		TextView weatherASCII = (TextView) findViewById(R.id.weather_ascii_string);
		
		final String curl = "http://api.wunderground.com/api/afd835494ae994bc/conditions/q/" + Utility.mUserZip + ".json";

		String mStrWeather = "";
      	
		// JSON Node names
      	final String TAG_OBSERVATION = "current_observation";
      	final String TAG_TEMP = "temp_f";
      	final String TAG_FEELS = "feelslike_f";
      	final String TAG_WEATHER = "weather";
      	final String TAG_WIND = "wind_string";
      	final String TAG_CURR_ICON = "icon";
      	
   		// Creating JSON Parser instance
  		JSONParser jParser = new JSONParser();
  		String temp = null;
  		String feels = null;
  		String weather = null;
  		String wind = null;
  		String cur_icon = null;
  		
  		JSONObject wjson = jParser.getJSONFromUrl(curl);
  		try {
			JSONObject observ = wjson.getJSONObject(TAG_OBSERVATION);
			temp = observ.getString(TAG_TEMP);
			feels = observ.getString(TAG_FEELS);
			weather = observ.getString(TAG_WEATHER);
			wind = observ.getString(TAG_WIND);
			cur_icon = observ.getString(TAG_CURR_ICON);
		}
  		catch (JSONException e) {
  			e.printStackTrace();
  		}
      
  		int tempVal = (int) Double.parseDouble(temp);
  		int feelsVal = (int) Double.parseDouble(feels);
  		
		wind = wind.replaceAll("[Mm][Pp][Hh]", "miles per hour");
		for(int i = 0; i < 2; i++) {
			wind = wind.replaceAll("([ \t\n\f\r][NSEW]*)N([NSEW]*[ \t\n\f\r])", "$1 north $2");
			wind = wind.replaceAll("([ \t\n\f\r][NSEW]*)S([NSEW]*[ \t\n\f\r])", "$1 south $2");
			wind = wind.replaceAll("([ \t\n\f\r][NSEW]*)E([NSEW]*[ \t\n\f\r])", "$1 east $2");
			wind = wind.replaceAll("([ \t\n\f\r][NSEW]*)W([NSEW]*[ \t\n\f\r])", "$1 west $2");
		}
  		
  		mStrWeather = "Currently, the temperature in " + Utility.mUserCity + " is " + tempVal + " degrees, ";
  		if(Math.abs(tempVal - feelsVal) > 2) mStrWeather = mStrWeather + "but it feels like " + feelsVal + ", ";
  		mStrWeather = mStrWeather + "under " + weather + " skies. The wind is " + wind + ". ";
 
  		// Parse current conditions and save
  		String parsed_current = Utility.parse_string(mStrWeather);	
  		Utility.mCurrent_cond = "WEATHER\n" + parsed_current +"\0"; 
  		
  		// Save cur_icon
  		String cur_icon_to_display = Utility.choose_pic(cur_icon);
  		Utility.mIcon_cur = cur_icon_to_display + "\0";
  		
  		
  		// Weather Forecast
    	final String furl = "http://api.wunderground.com/api/afd835494ae994bc/forecast/q/" + Utility.mUserZip + ".json";
    	// JSON Node names
    	final String TAG_FORECAST = "forecast";
    	final String TAG_TXTFORECAST = "txt_forecast";
    	final String TAG_DAY = "forecastday";
    	final String TAG_TEXT = "fcttext";
    	final String TAG_ICON = "icon";
    	
 		// Creating JSON Parser instance
		JSONParser fParser = new JSONParser();
		String fcttext = "";
		String forecast_icon = null;
		     		
		JSONObject fjson = fParser.getJSONFromUrl(furl);
		try 
		{
			JSONObject forecast = fjson.getJSONObject(TAG_FORECAST);
			JSONObject txtforecast = forecast.getJSONObject(TAG_TXTFORECAST);
			JSONArray days = txtforecast.getJSONArray(TAG_DAY);
			JSONObject d = days.getJSONObject(0);
			fcttext = d.getString(TAG_TEXT);
			forecast_icon = d.getString(TAG_ICON);
		}
		catch (JSONException e) {
			e.printStackTrace();
		}
        
		fcttext = fcttext.replaceAll("([0-9]+)F", "$1");
		fcttext = fcttext.replaceAll("[Mm][Pp][Hh]", "miles per hour");
		for(int i = 0; i < 2; i++) 
		{
			fcttext = fcttext.replaceAll("([ \t\n\f\r][NSEW]*)N([NSEW]*[ \t\n\f\r])", "$1 north $2");
			fcttext = fcttext.replaceAll("([ \t\n\f\r][NSEW]*)S([NSEW]*[ \t\n\f\r])", "$1 south $2");
			fcttext = fcttext.replaceAll("([ \t\n\f\r][NSEW]*)E([NSEW]*[ \t\n\f\r])", "$1 east $2");
			fcttext = fcttext.replaceAll("([ \t\n\f\r][NSEW]*)W([NSEW]*[ \t\n\f\r])", "$1 west $2");
		}
		mStrWeather = "";
		
		mStrWeather = "The forecast for today is: " + fcttext;
		
		// Parse forecast string and save
		String parsed_forecast = Utility.parse_string(mStrWeather);
		Utility.mForecast = "FORECAST\n" + parsed_forecast + "\0";
		
		// Save forecast icon
		String forecast_icon_to_display = Utility.choose_pic(forecast_icon);
		Utility.mIcon_forecast = forecast_icon_to_display + "\0";
		
		
		
		
		// Create hex string for debugging
		// Get hex value for each char
		String parsed = Utility.mCurrent_cond + Utility.mForecast;
		
		weatherTextView.setText(parsed);
		
		String ascii = "";
		int index = 0;
		
		for (index = 0; index < parsed.length(); index++)
		{
			char next = parsed.charAt(index);
			ascii = ascii + Integer.toHexString((int)next) + " ";
		}
		weatherASCII.setText(ascii);

		// Timer to exit the activity
		Timer timer = new Timer();
	    timer.schedule(new TimerTask() {

	    public void run() {
		  finish();
		  return;
	    }

	   }, 20000);
		
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_weather, menu);
		return true;
	}
	
	

}
