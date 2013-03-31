package edu.smwatch.bluetooth;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.Menu;

public class MainActivity extends Activity implements IBluetoothServiceEventReceiver {
	
	public static final int REQUEST_ENABLE_BT = 1001;
	
	public static final int SELECT_DEVICE_BT = 1002;
	
	BluetoothAdapter mBluetoothAdapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
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
		
		// Enable Bluetoth
		
		if (!mBluetoothAdapter.isEnabled()) {
		    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
		} else {
			bluetoothEnabled();
		}
		
		BluetoothService.initialize(getApplicationContext(), this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
			case REQUEST_ENABLE_BT: {
				if (!mBluetoothAdapter.isEnabled()) {
					// TODO: Show the same error screen but with a Bluetooth inactive message
				} else {
					bluetoothEnabled();
				}
				break;
			}
			case SELECT_DEVICE_BT: {
				if (resultCode == Activity.RESULT_OK) {
					// Get the device MAC address
					String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);

					// Und verbinden
					BluetoothService.connectToDevice(address);
				}
			}
			default: {
				super.onActivityResult(requestCode, resultCode, data);
				break;
			}
		}
	}

	 
	private void startSearchDeviceIntent() {
		Intent serverIntent = new Intent(this, DeviceListActivity.class);
		startActivityForResult(serverIntent, SELECT_DEVICE_BT);
	}

	@Override
	public void bluetoothEnabling() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void bluetoothEnabled() {
		startSearchDeviceIntent();
		
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
