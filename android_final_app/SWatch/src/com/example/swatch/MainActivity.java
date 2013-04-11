package com.example.swatch;

import java.util.Set;

import org.json.JSONException;
import org.json.JSONObject;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.StrictMode;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class MainActivity extends Activity {
	
	private Handler handler;
    private CommThread thread;
    private ProgressDialog dialog;

	public static final int REQUEST_ENABLE_BT = 1001;

	public static final int SELECT_DEVICE_BT = 1002;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
		StrictMode.setThreadPolicy(policy); 
      
		
		
		
		//Intent myIntent = new Intent(MainActivity.this, BTActivity.class);
    	//MainActivity.this.startActivity(myIntent);
		
		
		
		// Check if Bluetooth is available on device
		/*BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		if (mBluetoothAdapter == null) {
		    // Device does not support Bluetooth
		}
		
		// Enable Bluetooth
		if (!mBluetoothAdapter.isEnabled()) {
		    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
		}
		
		Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
		// If there are paired devices
		if (pairedDevices.size() > 0) {
		    // Loop through paired devices
		    for (BluetoothDevice device : pairedDevices) {
				// Add the name and address to an array adapter to show in a ListView
		        mArrayAdapter.add(device.getName() + "\n" + device.getAddress());
		    }
		}*/
		
		Button saveWeather = (Button) findViewById(R.id.save_weather);
        final EditText zipCode = (EditText) findViewById(R.id.zip_code);
        Button getWeather = (Button) findViewById(R.id.get_weather);
        
        Button saveHeadlines = (Button) findViewById(R.id.save_headlines);
        final EditText numHeadlines = (EditText) findViewById(R.id.number_headlines);
        Button getHeadlines = (Button) findViewById(R.id.get_headlines);
        
        Button getTime = (Button) findViewById(R.id.time);
        
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
        
        getWeather.setOnClickListener(new View.OnClickListener() {
        	
            @Override
            public void onClick(View v)
            {
            	Intent myIntent = new Intent(MainActivity.this, WeatherActivity.class);
            	MainActivity.this.startActivity(myIntent);
            }
        });
 
        saveHeadlines.setOnClickListener(new View.OnClickListener() {
        	
            @Override
            public void onClick(View v)
            {
            	Utility.mNumHeadlines = numHeadlines.getText().toString();
            	if(Integer.parseInt(Utility.mNumHeadlines) > 12)
            	{
            		Toast.makeText(MainActivity.this, "Max number of headlines is 12. Number of headlines will now be set to 12.", Toast.LENGTH_LONG).show();
            		Utility.mNumHeadlines = "12";
            	}
            	else
            	{
            		Toast.makeText(MainActivity.this, "Number of headlines set as " + Utility.mNumHeadlines, Toast.LENGTH_SHORT).show();
            	}
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
	@Override
    public void onStart() {
            super.onStart();
            CommThread.cancel();
            dialog = ProgressDialog.show(this, "Connecting", "Searching for a Bluetooth serial port...");
            thread = new CommThread(BluetoothAdapter.getDefaultAdapter(), dialog, handler);
            thread.start();
    }
	
	@Override
    public void onDestroy() {
		super.onDestroy();
            CommThread.cancel();
    }
}
