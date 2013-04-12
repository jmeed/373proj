package com.example.swatch;

import java.util.Timer;
import java.util.TimerTask;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;
import android.widget.Toast;

public class HeadlinesActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_headlines);
		
		// Ensure a valid number of headlines saved
		if (Utility.mNumHeadlines == null)
		{
			Toast.makeText(HeadlinesActivity.this, "Number of headlines is invalid.  Please save number and retry.", Toast.LENGTH_SHORT).show();
			finish();
			return;
			
		}
		
		System.out.println("in get headlines");
		
		TextView headlineTextView = (TextView) findViewById(R.id.headline_string);
		TextView headlineASCII = (TextView) findViewById(R.id.headline_ascii_string);
		
		String mHeadlineNews = "";
		// Get current headlines
    	final String url = "http://api.usatoday.com/articles/topnews/home?count=" + Utility.mNumHeadlines + "&days=0&page=0&encoding=json&api_key=jmfqkr26s7kh2saapy2spxxz";
    
    	// JSON Node names
    	final String TAG_STORIES = "stories";
    	final String TAG_DESCRIPTION = "description";
    	final String TAG_TITLE = "title";
    	
 		// Creating JSON Parser instance
		JSONParser fParser = new JSONParser();
		
		System.out.println("created parser");
		
		String title = null;
		String description = null;
		String headlineNews = "";
		     		
		JSONObject fjson = fParser.getJSONFromUrl(url);
		try {
			JSONArray stories = fjson.getJSONArray(TAG_STORIES);
			
			for (int i = 0; i < stories.length(); i++) {
				
				JSONObject story = stories.getJSONObject(i);
				title = story.getString(TAG_TITLE);
				description = story.getString(TAG_DESCRIPTION);
				
				headlineNews = headlineNews + (i + 1) + ") " + title + ": " + description + "\n";
			}
		}
		catch (JSONException e) {
			e.printStackTrace();
		}
		
		// Parse string
		String parsed = Utility.parse_string(headlineNews);
		mHeadlineNews = "HEADLINES\n" + parsed;
		headlineTextView.setText(mHeadlineNews);

		// Send over Bluetooth
		Utility.send_over_BT(mHeadlineNews);
		
		// Get hex value for each char
		String ascii = "";
		int index = 0;
		
		for (index = 0; index < mHeadlineNews.length(); index++)
		{
			char next = mHeadlineNews.charAt(index);
			ascii = ascii + Integer.toHexString((int)next) + " ";
		}
		headlineASCII.setText(ascii);
		
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
		getMenuInflater().inflate(R.menu.activity_headlines, menu);
		return true;
	}

}
