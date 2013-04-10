package com.example.swatch;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.util.Log;
import android.widget.Toast;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.util.UUID;

public final class BluetoothService {

	private static final String TAG = "BluetoothService";

	private static final UUID uuidSpp = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

	private static final String sync_message = "00\0";
	private static final int AUTHENTICATE = 0;

	private static Context applicationContext;

	private static boolean initialized;

	private static final IntentFilter broadcastIntentFilter = new IntentFilter();

	private static BroadcastReceiver broadcastReceiver;

	private BluetoothService() {
	}

	private static BluetoothAdapter btAdapter;

	private static IBluetoothServiceEventReceiver eventReceiver;

	private final static Handler eventReceiverHandler = new Handler();

	private static BluetoothDevice connectedDevice;

	private static BluetoothSocket connectedSocket;

	private static BufferedOutputStream outputStream;

	private static BufferedInputStream inputStream;

	private static byte[] buffer = new byte[1024];

	public static synchronized boolean initialize(Context applicationContext,
			IBluetoothServiceEventReceiver eventReceiver) {
		BluetoothService.eventReceiver = eventReceiver;

		if (initialized)
			return true;
		BluetoothService.applicationContext = applicationContext;

		btAdapter = BluetoothAdapter.getDefaultAdapter();
		if (btAdapter == null) {
			return false;
		}

		initialized = true;
		return true;
	}

	public static synchronized boolean bluetoothAvailable() {
		return btAdapter != null;
	}

	public static synchronized boolean bluetoothEnabled() {
		return btAdapter != null && btAdapter.isEnabled();
	}

	public static synchronized boolean requestEnableBluetooth(Activity activity) {
		if (bluetoothEnabled())
			return false;

		Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		activity.startActivityForResult(enableIntent,
				MainActivity.REQUEST_ENABLE_BT);
		return true;
	}

	public static synchronized void registerBroadcastReceiver(
			final Activity activity) {
		if (broadcastReceiver == null) {
			broadcastReceiver = new BroadcastReceiver() {
				@Override
				public void onReceive(Context context, Intent intent) {

					int currentState = intent.getIntExtra(
							BluetoothAdapter.EXTRA_STATE, -1);
					int lastState = intent.getIntExtra(
							BluetoothAdapter.EXTRA_PREVIOUS_STATE, -1);

					Log.v(TAG, "Bluetooth state change received: " + lastState
							+ " --> " + currentState);
					switch (currentState) {
					case BluetoothAdapter.STATE_TURNING_ON:
						onBluetoothEnabling();
						break;
					case BluetoothAdapter.STATE_ON:
						onBluetoothEnabled();
						break;
					case BluetoothAdapter.STATE_TURNING_OFF:
						onBluetoothDisabling();
						break;
					case BluetoothAdapter.STATE_OFF:
						onBluetoothDisabled();
						break;
					}

					if (currentState == BluetoothAdapter.STATE_ON)
						onBluetoothEnabled();
					if (currentState == BluetoothAdapter.STATE_TURNING_OFF)
						onBluetoothDisabling();
				}
			};

			broadcastIntentFilter
					.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
		}
		activity.registerReceiver(broadcastReceiver, broadcastIntentFilter);
	}

	public static synchronized void unregisterBroadcastReceiver(
			Activity activity) {
		if (broadcastReceiver == null)
			return;
		activity.unregisterReceiver(broadcastReceiver);
	}

	private static synchronized void onBluetoothEnabling() {
		assert eventReceiver != null;
		eventReceiverHandler.post(new Runnable() {
			@Override
			public void run() {
				eventReceiver.bluetoothEnabling();
			}
		});
	}

	private static synchronized void onBluetoothEnabled() {
		assert eventReceiver != null;
		eventReceiverHandler.post(new Runnable() {
			@Override
			public void run() {
				eventReceiver.bluetoothEnabled();
			}
		});
	}

	private static synchronized void onBluetoothDisabling() {
		assert eventReceiver != null;
		eventReceiverHandler.post(new Runnable() {
			@Override
			public void run() {
				eventReceiver.bluetoothDisabling();
			}
		});
	}

	private static synchronized void onBluetoothDisabled() {
		assert eventReceiver != null;
		eventReceiverHandler.post(new Runnable() {
			@Override
			public void run() {
				eventReceiver.bluetoothDisabled();
			}
		});
	}

	public static synchronized void connectToDevice(final String macAddress) {
		assert eventReceiver != null;

		disconnect();

		BluetoothDevice device = btAdapter.getRemoteDevice(macAddress);
		Log.i(TAG, "Bluetooth-Ger??t ausgew??hlt: " + device.getName() + "; "
				+ device.getAddress());
		connectedDevice = device;

		final String deviceName = device.getName();
		eventReceiverHandler.post(new Runnable() {
			@Override
			public void run() {
				eventReceiver.connectedTo(deviceName == null ? "unnamed"
						: deviceName, macAddress);
			}
		});

		try {
			BluetoothSocket socket = device
					.createRfcommSocketToServiceRecord(uuidSpp);
			connectedSocket = socket;
			if (socket == null) {
				Log.e(TAG, "Konnte Bluetooth-Socket nicht erzeugen!"); // TODO:
																		// UI
				return;
			}

			if (btAdapter.isDiscovering())
				btAdapter.cancelDiscovery();

			try {
				Log.i(TAG, "Connecting Socket to " + device.getName()); // TODO:UI
				socket.connect();
			} catch (IOException e) {
				Log.e(TAG, "Konnte Verbindung nicht herstellen.", e); // TODO:UI
				return;
			}

			try {
				InputStream realInputStream = socket.getInputStream();
				if (realInputStream == null) {
					Log.e(TAG, "Konnte Input-Stream nicht erzeugen"); // TODO:UI
					return;
				}
				inputStream = new BufferedInputStream(realInputStream);
			} catch (IOException e) {
				Log.e(TAG, "Konnte Input-Stream nicht erzeugen", e); // TODO: UI
				return;
			}

			try {
				OutputStream realOutputStream = socket.getOutputStream();
				if (realOutputStream == null) {
					Log.e(TAG, "Konnte Output-Stream nicht erzeugen"); // TODO:UI
					return;
				}
				outputStream = new BufferedOutputStream(realOutputStream);
			} catch (IOException e) {
				Log.e(TAG, "Konnte Output-Stream nicht erzeugen", e); // TODO:UI
				return;
			}

			sendSyncMessage();

		} catch (IOException e) {
			e.printStackTrace(); // TODO: An UI weitergeben!
		} catch (NullPointerException e) {
			Log.e(TAG, "Nullreferenz-Ausnahmefehler!", e); // TODO: UI
		}
	}

	public static synchronized void disconnect() {

		if (outputStream != null)
			try {
				outputStream.flush();
				outputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (NullPointerException e) {
			}
		outputStream = null;

		if (inputStream != null)
			try {
				inputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (NullPointerException e) {
			}
		inputStream = null;

		if (connectedSocket != null)
			try {
				connectedSocket.getOutputStream().close();
				connectedSocket.getInputStream().close();
				connectedSocket.close();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (NullPointerException e) {
			}
		connectedSocket = null;

		connectedDevice = null;
	}

	private static synchronized void sendSyncMessage() {
		assert outputStream != null;
		try {
			// Send the sync message
			outputStream.write(sync_message.getBytes());
			outputStream.flush();

			// Receive the sync message to ensure pairing with the right device
			int length = readFromTarget();
			int opcode = process_bl_msg(length);
			if (opcode == AUTHENTICATE) {
				System.out.println("Success");
			} else {
				System.out.println("Failure");
			}

		} catch (IOException e) {
			Log.e(TAG, "Fehler beim Senden der Sync-Nachricht", e);
		}
	}

	public static boolean isConnected() {
		return connectedSocket != null && outputStream != null;
	}

	public static synchronized void sendToTarget(String message) {
		try {
			outputStream.write(message.getBytes());
			outputStream.write('\r');
			outputStream.write('\n');
			outputStream.flush();
		} catch (IOException e) {
		} catch (NullPointerException e) {
		}
	}

	public static int readFromTarget() {
		int index = 0;
		byte[] tmp_buff = new byte[1024];
		while(true) {
			// Receive the sync message to ensure pairing with the right device
			int bytes = 0; // bytes returned from read()
			try {
				bytes = inputStream.read(tmp_buff);

				// Copy to permenant buffer
				for (int i = 0; i < bytes; i++) {
					buffer[index] = tmp_buff[i];
					index++;
				}

				// If the last byte is \0 we are done
				if(buffer[index - 1] == '\0') {
					break;
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return index;
	}

	public static int process_bl_msg(int length) {
		assert length >= 3;
		int opcode = 10 * (buffer[0] - '0');
		opcode = opcode + (buffer[1] - '0');

		System.out.println("BL recieved. opcode " + opcode + " " + length);

		switch (opcode) {
		case AUTHENTICATE:
			break;
		default:
			assert false;
			break;
		}
		return opcode;
	}
}
