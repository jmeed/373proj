package com.example.swatch;

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
		
		mHeadlineNews = "HEADLINES\n" + headlineNews;
		headlineTextView.setText(mHeadlineNews);
		
		// Get hex value for each char
		String ascii = "";
		int length = 0;
		int start_for = 0;
		int index = 0;
		while(true)
		{
			for (index = start_for; index < mHeadlineNews.length(); index++)
			{
				char next = mHeadlineNews.charAt(index);
				ascii = ascii + Integer.toHexString((int)next) + " ";
				length++;
				
				// Split string into substrings to send to screen
				if(length >= 253)
				{
					start_for = index + 1;
					length = 0;
					break;
				}
			}
			headlineASCII.setText(ascii);
			// SEND TO SCREEN HERE
			ascii = "";
			if (index >= mHeadlineNews.length())
			{
				break;
			}
		}
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_headlines, menu);
		return true;
	}

}
