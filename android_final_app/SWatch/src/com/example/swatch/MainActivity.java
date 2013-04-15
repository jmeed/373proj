package com.example.swatch;

import org.json.JSONException;
import org.json.JSONObject;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.bluetooth.BluetoothAdapter;

@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class MainActivity extends Activity {

    private CommThread thread;
    private ProgressDialog dialog;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		// Set thread policy to allow parsing
		StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
		StrictMode.setThreadPolicy(policy);
		
		CommThread.cancel();
        dialog = ProgressDialog.show(this, "Connecting", "Searching for SMWatch...");
        thread = new CommThread(BluetoothAdapter.getDefaultAdapter(), dialog, mHandler);
        thread.start();
		
		Button saveWeather = (Button) findViewById(R.id.save_weather);
        final EditText zipCode = (EditText) findViewById(R.id.zip_code);
        Button getWeather = (Button) findViewById(R.id.get_weather);
        
        Button getHeadlines = (Button) findViewById(R.id.get_headlines);
        
        Button getTime = (Button) findViewById(R.id.time);
        
        // START verify zip code ----------------------------------------------
        zipCode.setText(Utility.mUserZip);
        saveWeather.setOnClickListener(new View.OnClickListener() {
        	
            @Override
            public void onClick(View v)
            {
            	String zip = zipCode.getText().toString();
                final String url = "http://api.wunderground.com/api/afd835494ae994bc/conditions/q/" + zip + ".json";
              	
              	// JSON Node names
              	final String TAG_OBSERVATION = "current_observation";
              	final String TAG_LOCATION = "display_location";
              	final String TAG_CITY = "city";
              	final String TAG_STATE = "state";
              	
          		JSONParser jParser = new JSONParser();          		
          		JSONObject wjson = jParser.getJSONFromUrl(url);
          		try 
          		{
          				JSONObject observ = wjson.getJSONObject(TAG_OBSERVATION);
          				JSONObject location = observ.getJSONObject(TAG_LOCATION);
          				Utility.mUserCity = location.getString(TAG_CITY);
          				Utility.mUserState = location.getString(TAG_STATE);
          		}
          		catch (JSONException e) 
          		{
          			Utility.mUserZip = "";
          			Utility.mUserCity = "";
          			Utility.mUserState = "";
          			Toast toast = Toast.makeText(getApplicationContext(), "Invalid zip code", Toast.LENGTH_SHORT );
        			toast.setGravity(Gravity.BOTTOM, 0, 0);
        			toast.show();
          			e.printStackTrace();
          		}
          		
          		if(Utility.mUserCity.length() > 0 && Utility.mUserState.length() > 0) 
          		{
          			Utility.mUserZip = zip;
          			Toast toast = Toast.makeText(getApplicationContext(), "Setting weather for " + Utility.mUserCity + " , " + Utility.mUserState, Toast.LENGTH_SHORT );
        			toast.setGravity(Gravity.BOTTOM, 0, 0);
        			toast.show();
          		}
              	
            }
            
        });
        // END verify zip code -------------------------------------------
                
        getWeather.setOnClickListener(new View.OnClickListener() {
        	
            @Override
            public void onClick(View v)
            {
            	Intent myIntent = new Intent(MainActivity.this, WeatherActivity.class);
            	MainActivity.this.startActivity(myIntent);
            }
        });
     
        
        
        
        getHeadlines.setOnClickListener(new View.OnClickListener() {
        	
            @Override
            public void onClick(View v)
            {
            	Intent myIntent = new Intent(MainActivity.this, HeadlinesActivity.class);
            	MainActivity.this.startActivity(myIntent);
            }
        });
        
        getTime.setOnClickListener(new View.OnClickListener() {
        	
            @Override
            public void onClick(View v)
            {
            	Intent myIntent = new Intent(MainActivity.this, TimeActivity.class);
            	MainActivity.this.startActivity(myIntent);
            }
        });
    }
	
	Handler mHandler = new Handler(){
      	 @Override
      	    public void handleMessage(Message msg) {
      		byte[] readBuf = (byte[]) msg.obj;
      		String readMessage = new String(readBuf, 0, msg.arg1);
      		String headline = null;
      	        switch (msg.what) 
      	        {
      	        case 0:
      	        	if (readMessage.charAt(1) == '0')
      	        	{
      	        		Intent myIntent = new Intent(MainActivity.this, HeadlinesActivity.class);
      	        		MainActivity.this.startActivity(myIntent);
      	        		Utility.mHeadline_counter = -1;
      	        	}
      	        	else if (readMessage.charAt(1) == '1')
      	        	{
      	        		Utility.mHeadline_counter++;
      	        		if (Utility.mHeadline_counter > 11)
      	        			Utility.mHeadline_counter = 0;
      	        		headline = Utility.get_headline(Utility.mHeadline_counter);
      	        		System.out.println("NUM: "+Utility.mHeadline_counter);
      	        		System.out.println("get next headline "+headline + headline.length());
      	        		CommThread.write(headline.getBytes());      	        		
      	        	}
      	        	else if (readMessage.charAt(1) == '2')
      	        	{
      	        		System.out.println("get previous headline "+Utility.mCurrent_cond);
      	        		Utility.mHeadline_counter--;
      	        		if (Utility.mHeadline_counter < 0)
      	        			Utility.mHeadline_counter = 11;
      	        		headline = Utility.get_headline(Utility.mHeadline_counter);
      	        		System.out.println("NUM: "+Utility.mHeadline_counter);
      	        		System.out.println("get next headline "+headline + headline.length());	        		
      	        		CommThread.write(headline.getBytes());      	        		
      	        	}
      	        	break;

      	        case 1:
      	        	if (readMessage.charAt(1) == '0')
      	        	{
      	        		Intent myIntent1 = new Intent(MainActivity.this, WeatherActivity.class);
      	        		MainActivity.this.startActivity(myIntent1);
      	        	}
      	        	else if (readMessage.charAt(1) == '1')
      	        	{
      	        		System.out.println("send current conditions "+Utility.mCurrent_cond);
      	        		CommThread.write(Utility.mIcon_cur.getBytes());
      	        		CommThread.write(Utility.mCurrent_cond.getBytes());
      	        	}
      	        	else if (readMessage.charAt(1) == '2')
      	        	{
      	        		System.out.println("send forecast" + Utility.mForecast);
      	        		CommThread.write(Utility.mIcon_forecast.getBytes());
      	        		CommThread.write(Utility.mForecast.getBytes());
      	        	}
                	System.out.println("Case: weather "+ readMessage);
  	                break;

      	          case 2:
      	        	if (readMessage.charAt(1) == '0')
      	        	{
      	        		Intent myIntent2 = new Intent(MainActivity.this, TimeActivity.class);
      	        		MainActivity.this.startActivity(myIntent2);
      	        	}
      	        	else if (readMessage.charAt(1) == '1')
      	        	{
      	        		System.out.println("send TIME" + Utility.mTime);
      	        		CommThread.write(Utility.mTime.getBytes());
      	        	}
      	            break;

      	        }
      	    }
      	};
	
	@Override
    public void onStart() {
            super.onStart();
            
    }
	
	@Override
    public void onDestroy() {
		super.onDestroy();
            CommThread.cancel();
    }
}
