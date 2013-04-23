package com.example.swatch;

import org.json.JSONException;
import org.json.JSONObject;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.bluetooth.BluetoothAdapter;

@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class MainActivity extends Activity {

	public CommThread thread;
    public ProgressDialog dialog;
    public Activity activity;
    public String track;
    public IntentFilter iF;
    public BroadcastReceiver mReceiver;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		activity = this;
		setContentView(R.layout.activity_main);
		// Set thread policy to allow parsing
		StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
		StrictMode.setThreadPolicy(policy);
		
		
		mReceiver = new BroadcastReceiver() {
			 
	        @Override
	        public void onReceive(Context arg0, Intent intent) {
	 
	            String action = intent.getAction();
	            String cmd = intent.getStringExtra("command");
	            String artist = intent.getStringExtra("artist");
	            String album = intent.getStringExtra("album");
	            track = intent.getStringExtra("track") + '\0';
	            // have fun with it <img src="http://s0.wp.com/wp-includes/images/smilies/icon_smile.gif?m=1129645325g" alt=":)" class="wp-smiley" scale="0"> 
	        }
		};
		
		// Music
		iF = new IntentFilter();
		 
        // Read action when music player changed current song
        // I just try it with stock music player form android
 
        // stock music player
        iF.addAction("com.android.music.metachanged");
 
     
        
        registerReceiver(mReceiver, iF);
		
		
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
        
//		CommThread.cancel();
		dialog = ProgressDialog.show(this, "Connecting", "Searching for M-Watch...");
        thread = new CommThread(BluetoothAdapter.getDefaultAdapter(), dialog, mHandler, getApplicationContext(), this);
        thread.start();
//        thread.join();
        
    }
	
	Handler mHandler = new Handler(){
      	 @Override
      	    public void handleMessage(Message msg) {
      		byte[] readBuf = (byte[]) msg.obj;
      		//Utility.clear_BT();
      		String readMessage = new String(readBuf);
      		if(readMessage.length() < 3)
  	        {
  	        	System.out.println("INCORRECT MESS "+readMessage);
  	        	Toast.makeText(MainActivity.this, "Application Error. Error code: 0b758x", Toast.LENGTH_SHORT).show();
  				finish();
  				return;
  	        } else if (readMessage.startsWith(new String("show"))) {
  	        	if(!dialog.isShowing()) {
  	        		System.out.println("Showing dialog!");
  	        		dialog = ProgressDialog.show(activity, "Connecting", "Searching for M-Watch...");
  	        	}
  	        	return;
  	        }
      		
      		System.out.println("READ MESS MAIN "+readMessage);
      		String headline = null;
      	        switch (msg.what) 
      	        {
      	        case 0:
      	        	if (readMessage.charAt(1) == '0')
      	        	{
      	        		if (Utility.how_many_sends >= Utility.how_many_sends_total)
      	        		{
      	        			String error = "Bluetooth error\nError code 0r327\0";
      	        			CommThread.write(error.getBytes());
      	        			break;
      	        		}
        	        	  String to_send = null;
        	        	  to_send = Utility.get_to_send();
        	        	  CommThread.write(to_send.getBytes());
        	        	  Utility.how_many_sends++;
      	        	}
      	        	else if (readMessage.charAt(1) == '1')
      	        	{
      	        		Intent myIntent2 = new Intent(MainActivity.this, TimeActivity.class);
      	        		MainActivity.this.startActivity(myIntent2);   		
      	        	}
      	        	break;
      	        case 2:
      	        	if (readMessage.charAt(1) == '0')
      	        	{
      	        		Intent myIntent = new Intent(MainActivity.this, HeadlinesActivity.class);
      	        		MainActivity.this.startActivity(myIntent);
      	        	}
      	        	else if (readMessage.charAt(1) == '1')
      	        	{
      	        		Utility.mHeadline_counter++;
      	        		if (Utility.mHeadline_counter > 11)
      	        			Utility.mHeadline_counter = 0;
      	        		headline = Utility.get_headline(Utility.mHeadline_counter);
      	        		Utility.set_BT("21"+headline);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;      	
      	        	}
      	        	else if (readMessage.charAt(1) == '2')
      	        	{
      	        		Utility.mHeadline_counter--;
      	        		if (Utility.mHeadline_counter < 0)
      	        			Utility.mHeadline_counter = 11;
      	        		headline = Utility.get_headline(Utility.mHeadline_counter);
      	        		Utility.set_BT("22"+headline);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;    	        		
      	        	}
      	        	break;
      	        case 3:
      	        	if (readMessage.charAt(1) == '0')
      	        	{
      	        		Intent myIntent1 = new Intent(MainActivity.this, WeatherActivity.class);
      	        		MainActivity.this.startActivity(myIntent1);
      	        	}
      	        	else if (readMessage.charAt(1) == '1')
      	        	{
      	        		Utility.set_BT("31" + Utility.mIcon_forecast + Utility.mForecast);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;
      	        	}
  	                break;
      	          case 4:
      	        	if (readMessage.charAt(1) == '0')
      	        	{
      	        		Intent i = new Intent("com.android.music.musicservicecommand");
      	        		i.putExtra("command", "pause");
      	        		sendBroadcast(i);
      	        		System.out.println(track);
      	        		Utility.set_BT("40" + track);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;
      	        		
      	        	}
      	        	else if (readMessage.charAt(1) == '1')
      	        	{
      	        		Intent i = new Intent("com.android.music.musicservicecommand");
      	        		i.putExtra("command", "play");
      	        		sendBroadcast(i);
      	        		System.out.println(track);
      	        		Utility.set_BT("41" + track);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;
      	        	}
      	        	else if (readMessage.charAt(1) == '2')
      	        	{
      	        		Intent i = new Intent("com.android.music.musicservicecommand");
      	        		i.putExtra("command", "pause");
      	        		sendBroadcast(i);
      	        		System.out.println(track);
      	        		Utility.set_BT("42" + track);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;
      	        	}
      	        	else if (readMessage.charAt(1) == '3')
      	        	{
      	        		Intent i = new Intent("com.android.music.musicservicecommand");
      	        		i.putExtra("command", "next");
      	        		sendBroadcast(i);
      	        		System.out.println(track);
      	        		Utility.set_BT("43" + track);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;
      	        	}
      	        	else if (readMessage.charAt(1) == '4')
      	        	{
      	        		Intent i = new Intent("com.android.music.musicservicecommand");
      	        		i.putExtra("command", "previous");
      	        		System.out.println(track);
      	        		Utility.set_BT("44" + track);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;
      	        	}
      	        	else if (readMessage.charAt(1) == '5')
      	        	{
      	        		System.out.println("THE 45: " + track);
      	        		Utility.set_BT("45" + track);
      	        		CommThread.write(Utility.to_send_0.getBytes());
      	        		Utility.how_many_sends = 1;
      	        	}
      	            break;
      	        }
      	    }
      	};
	
	@Override
    public void onStart() {
            super.onStart();
            /*new CountDownTimer(30000, 1000) {
            	public void onFinish(){
            		CommThread.cancel();
            		//dialog = ProgressDialog.show();
            		try {
						Thread.sleep(3);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
            		thread = new CommThread(BluetoothAdapter.getDefaultAdapter(), dialog, mHandler);
            		thread.start();
            		this.start();
            	}

				@Override
				public void onTick(long arg0) {
					// TODO Auto-generated method stub
					
				}
               
             }.start();*/
    }
	
	@Override
    public void onDestroy() {
		super.onDestroy();
            CommThread.cancel();
    }
}
