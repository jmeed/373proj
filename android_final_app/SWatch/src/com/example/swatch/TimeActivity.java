package com.example.swatch;

import java.util.Calendar;
import java.util.Timer;
import java.util.TimerTask;

import android.os.Bundle;
import android.app.Activity;
import android.text.format.Time;
import android.view.Menu;
import android.widget.TextView;

public class TimeActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_time);
		
		TextView timeTextView = (TextView) findViewById(R.id.time_string);
		TextView timeASCII = (TextView) findViewById(R.id.time_ascii_string);
		
		String dummy_now = "";
		String ascii = "";
		dummy_now = java.text.DateFormat.getDateTimeInstance().format(Calendar.getInstance().getTime());
		String now = dummy_now.substring(0,12) + '\n' + dummy_now.substring(13);
		for (int index = 0; index < now.length(); index++)
		{
			char next = now.charAt(index);
			ascii = ascii + Integer.toHexString((int)next) + " ";
		}
		
		
		timeTextView.setText(now);
		timeASCII.setText(ascii);
		CommThread.write(now.getBytes());
		
		Timer timer = new Timer();
	    timer.schedule(new TimerTask() {

	    public void run() {
		   finish();
		   return;
	   }

	}, 5000);
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_time, menu);
		return true;
	}

}
