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
		
		System.out.println("in get headlines");
		
		Utility.mHeadline_counter = -1;
		
		TextView headlineTextView = (TextView) findViewById(R.id.headline_string);
		TextView headlineASCII = (TextView) findViewById(R.id.headline_ascii_string);
		
		String mHeadlineNews = "";
		// Get current headlines
    	final String url = "http://api.usatoday.com/articles/topnews/home?count=12&days=0&page=0&encoding=json&api_key=jmfqkr26s7kh2saapy2spxxz";
    
    	// JSON Node names
    	final String TAG_STORIES = "stories";
    	final String TAG_DESCRIPTION = "description";
    	final String TAG_TITLE = "title";
    	
 		// Creating JSON Parser instance
		JSONParser fParser = new JSONParser();
		
		System.out.println("created parser");
		
		String title = null;
		String description = null;
		
		String h0 = "";
		String h1 = "";
		String h2 = "";
		String h3 = "";
		String h4 = "";
		String h5 = "";
		String h6 = "";
		String h7 = "";
		String h8 = "";
		String h9 = "";
		String h10 = "";
		String h11 = "";
		
		     		
		JSONObject fjson = fParser.getJSONFromUrl(url);
		try {
			JSONArray stories = fjson.getJSONArray(TAG_STORIES);
			
			for (int i = 0; i < stories.length(); i++) {
				
				JSONObject story = stories.getJSONObject(i);
				title = story.getString(TAG_TITLE);
				description = story.getString(TAG_DESCRIPTION);
				
				if (i == 0)
					h0 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 1)
					h1 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 2)
					h2 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 3)
					h3 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 4)
					h4 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 5)
					h5 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 6)
					h6 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 7)
					h7 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 8)
					h8 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 9)
					h9 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 10)
					h10 = (i + 1) + ") " + title + ": " + description + "\n";
				else if (i == 11)
					h11 = (i + 1) + ") " + title + ": " + description + "\n";
			}
		}
		catch (JSONException e) {
			e.printStackTrace();
		}
		
		String parsed = Utility.parse_string(h0);
		Utility.mH0 = parsed + "\0";
		
		Utility.mHeadline_counter++;
		String headline = Utility.get_headline(Utility.mHeadline_counter);
  		//System.out.println("NUM: "+Utility.mHeadline_counter);
  		//System.out.println("get next headline "+headline + headline.length());
  		Utility.set_BT("20"+headline);
  		CommThread.write(Utility.to_send_0.getBytes());
  		Utility.how_many_sends = 1;  	        		
		
		parsed = Utility.parse_string(h1);
		Utility.mH1 = parsed + "\0";
		
		parsed = Utility.parse_string(h2);
		Utility.mH2 = parsed + "\0";
		
		parsed = Utility.parse_string(h3);
		Utility.mH3 = parsed + "\0";
		
		parsed = Utility.parse_string(h4);
		Utility.mH4 = parsed + "\0";
		
		parsed = Utility.parse_string(h5);
		Utility.mH5 = parsed + "\0";
		
		parsed = Utility.parse_string(h6);
		Utility.mH6 = parsed + "\0";
		
		parsed = Utility.parse_string(h7);
		Utility.mH7 = parsed + "\0";
		
		parsed = Utility.parse_string(h8);
		Utility.mH8 = parsed + "\0";
		
		parsed = Utility.parse_string(h9);
		Utility.mH9 = parsed + "\0";
		
		parsed = Utility.parse_string(h10);
		Utility.mH10 = parsed + "\0";
		
		parsed = Utility.parse_string(h11);
		Utility.mH11 = parsed + "\0";
		
		
		mHeadlineNews = Utility.mH0 + Utility.mH1 + Utility.mH2 + Utility.mH3 + Utility.mH4 + Utility.mH5 + Utility.mH6 + Utility.mH7 + Utility.mH8
				+ Utility.mH9 + Utility.mH10 + Utility.mH11;
		
		headlineTextView.setText(mHeadlineNews);

		// Get hex value for each char for debugging
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
