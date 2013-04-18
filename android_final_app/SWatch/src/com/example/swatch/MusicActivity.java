package com.example.swatch;

import android.os.Bundle;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;
import android.view.Menu;

public class MusicActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_music);
		
		IntentFilter iF = new IntentFilter();

		 

        // Read action when music player changed current song

        // I just try it with stock music player form android

 

        // stock music player

        iF.addAction("com.android.music.metachanged");

 

        // MIUI music player

        iF.addAction("com.miui.player.metachanged");

 

        // HTC music player

        iF.addAction("com.htc.music.metachanged");

 

        // WinAmp

        iF.addAction("com.nullsoft.winamp.metachanged");

 

        // MyTouch4G

        iF.addAction("com.real.IMP.metachanged");

        

        BroadcastReceiver mReceiver = new BroadcastReceiver() {


        	@Override

        	public void onReceive(Context context, Intent intent) {

        	String action = intent.getAction();

            String cmd = intent.getStringExtra("command");

            Log.d("mIntentReceiver.onReceive ", action + " / " + cmd);

            String artist = intent.getStringExtra("artist");

            String album = intent.getStringExtra("album");

            String track = intent.getStringExtra("track");

            Log.d("Music", artist + ":" + album + ":" + track);

           

            Intent i = new Intent("com.android.music.musicservicecommand");



            i.putExtra("command", "togglepause");

            sendBroadcast(i);

           

            i.putExtra("command", "play");

            sendBroadcast(i);

           

            i.putExtra("command", "next");

            sendBroadcast(i);

           

            // have fun with it <img src="http://s0.wp.com/wp-includes/images/smilies/icon_smile.gif?m=1129645325g" alt=":)" class="wp-smiley" scale="0"> 

        }



};

 

        registerReceiver(mReceiver, iF);


		
		
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_music, menu);
		return true;
	}

}
