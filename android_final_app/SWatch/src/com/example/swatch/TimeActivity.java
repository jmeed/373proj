package com.example.swatch;

import java.util.Calendar;
import java.util.Timer;
import java.util.TimerTask;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;

public class TimeActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_time);
		
		TextView timeTextView = (TextView) findViewById(R.id.time_string);
		TextView timeASCII = (TextView) findViewById(R.id.time_ascii_string);
		
		String ascii = "";
		long rightNow = Calendar.getInstance().getTimeInMillis()/1000;
		String time = String.valueOf(rightNow);
		
		for (int index = 0; index < time.length(); index++)
		{
			char next = time.charAt(index);
			ascii = ascii + Integer.toHexString((int)next) + " ";
		}
		
		
		timeTextView.setText(time);
		timeASCII.setText(ascii);
		
		// Save Time
		Utility.mTime = "01" + time + "\0";
		CommThread.write(Utility.mTime.getBytes());
		
		System.out.println(Utility.mTime);
		Timer timer = new Timer();
	    timer.schedule(new TimerTask() {

	    public void run() {
		   finish();
		   return;
	   }

	}, 7000);
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_time, menu);
		return true;
	}

}
