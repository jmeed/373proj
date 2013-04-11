package com.example.swatch;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.Menu;

public class BTActivity extends Activity implements IBluetoothServiceEventReceiver {

	public static final int REQUEST_ENABLE_BT = 1001;

	public static final int SELECT_DEVICE_BT = 1002;
	BluetoothAdapter mBluetoothAdapter;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		
		setContentView(R.layout.activity_bt);
		
		// Enable Bluetoeth

		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		if (mBluetoothAdapter == null) {
			// TODO: Show a big warning sign and an error
		    // Device does not support Bluetooth
			AlertDialog alertDialog = new AlertDialog.Builder(this).create();
			alertDialog.setTitle("No Bluetooth");
			alertDialog.setMessage("Your device does not appear to have a bluetooth module");
			alertDialog.setButton(AlertDialog.BUTTON_POSITIVE, "OK", new DialogInterface.OnClickListener() {
			   public void onClick(DialogInterface dialog, int which) {
				   dialog.dismiss();
			   }
			});
			// Set the Icon for the Dialog
			//alertDialog.setIcon(R.drawable.icon);
			alertDialog.show();
		}
		
		if (!mBluetoothAdapter.isEnabled()) {
		    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
		} else {
			bluetoothEnabled();
		}

		BluetoothService.initialize(getApplicationContext(), this);
		//finish();
		//return;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_bt, menu);
		return true;
	}

	@Override
	public void bluetoothEnabling() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void bluetoothEnabled() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void bluetoothDisabling() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void bluetoothDisabled() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void connectedTo(String name, String address) {
		// TODO Auto-generated method stub
		
	}

}
