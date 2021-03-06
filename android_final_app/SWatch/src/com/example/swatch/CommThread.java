package com.example.swatch;

/*
 * Copyright 2010 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Set;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
class CommThread extends Thread {
	private static BluetoothSocket socket;
	private static InputStream istream;
	private static OutputStream ostream;
	private static Handler handler;
	private static ProgressDialog dialog;
	private static BluetoothAdapter adapter;
	private static Context context;
	private static MainActivity activity;
	private static byte[] buffer = new byte[1024];

	public CommThread(BluetoothAdapter adapter, ProgressDialog dialog,
			Handler handler, Context context, MainActivity activity) {
		this.handler = handler;
		this.dialog = dialog;
		this.adapter = adapter;
		this.context = context;
		this.activity = activity;
	}

	public void run() {
		while(true) {
			if (adapter == null)
				return;
			CommThread.cancel();
			buffer[0] = 's';
			buffer[1] = 'h';
			buffer[2] = 'o';
			buffer[3] = 'w';
			handler.obtainMessage(0, buffer).sendToTarget();

			Set<BluetoothDevice> devices = adapter.getBondedDevices();
			BluetoothDevice device = null;
			for (BluetoothDevice curDevice : devices) {
				if (curDevice.getName().matches("RN42-21EE")) {
					device = curDevice;
					System.out.println("Device found. Breaking!");
					break;
				}
			}
			if (device == null) {
				device = adapter.getRemoteDevice("00:06:66:03:A7:52");
				System.out.println("Device not found");
			}

			BroadcastReceiver mReceiver = new BroadcastReceiver() {

				@Override
				public void onReceive(Context arg0, Intent arg1) {
					// TODO Auto-generated method stub
					String msg = arg1.getAction();
					if (msg == "android.bluetooth.device.action.ACL_DISCONNECTED") {
						System.out.println("Disconnected!!!");
					}

				}
			};

			context.registerReceiver(mReceiver, new IntentFilter(
					BluetoothDevice.ACTION_ACL_CONNECTED));
			context.registerReceiver(mReceiver, new IntentFilter(
					BluetoothDevice.ACTION_ACL_DISCONNECTED));

			while (true) {
				try {
					Method m = null;
					try {
						m = device.getClass().getMethod("createRfcommSocket",
								new Class[] { int.class });
					} catch (NoSuchMethodException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					socket = (BluetoothSocket) m.invoke(device, 1);

					// socket =
					// device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
					socket.connect();
					break;
				} catch (IOException e) {
					socket = null;
					System.out.println("socket unsuccessfully created");
				} catch (IllegalArgumentException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IllegalAccessException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (InvocationTargetException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			if (socket == null)
				return;

			InputStream tmpIn = null;
			OutputStream tmpOut = null;

			try {
				tmpIn = socket.getInputStream();
				tmpOut = socket.getOutputStream();
			} catch (IOException e) {
			}

			istream = tmpIn;
			ostream = tmpOut;

			if (activity.dialog != null && activity.dialog.isShowing())
				activity.dialog.dismiss();
			try {
				Thread.sleep(10000);
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}

			int act = -1;
			// Read from Bluetooth always
			while (true) {
				System.out.println("in while");
				act = readFromTarget();
				if (act == -2) {
					break; // Break if reading failed. This means disconnection was
							// detected
				}
				handler.obtainMessage(act, buffer).sendToTarget();
			}
		}
	}

	/* Call this from the main Activity to send data to the remote device */
	public static void write(byte[] bytes) {
		try {
			ostream.write(bytes);
		} catch (IOException e) {
			Log.e("CommThread.write", "exception during write", e);
		}
	}

	/* Call this from the main Activity to shutdown the connection */
	public static void cancel() {
		try {
			if (socket != null)
				socket.close();
		} catch (IOException e) {
		}
	}

	public static void ensure_connection() {

		Thread check = new Thread() {
			@Override
			public void run() {
				while (true) {
					if (!socket.isConnected()) {
						System.out.println("socket is not connected");

					}
				}
			}
		};
		check.start();
	}

	public static void ensure_connection_helper() {
		if (socket != null)
			return;
		else {
			System.out.println("Reconnecting connecting");
			if (adapter == null) {
				System.out.println("Adapter is null. Error!");
				return;
			}

			Set<BluetoothDevice> devices = adapter.getBondedDevices();
			BluetoothDevice device = null;
			for (BluetoothDevice curDevice : devices) {
				if (curDevice.getName().matches("RN42-21EE")) {
					device = curDevice;
					System.out.println("Device found. Breaking!");
					break;
				}
			}
			if (device == null) {
				device = adapter.getRemoteDevice("00:06:66:03:A7:52");
				System.out.println("Device not found");
			}

			try {
				Method m = null;
				try {
					m = device.getClass().getMethod("createRfcommSocket",
							new Class[] { int.class });
				} catch (NoSuchMethodException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				socket = (BluetoothSocket) m.invoke(device, 1);
				socket.connect();
			} catch (IOException e) {
				socket = null;
				System.out.println("socket unsuccessfully created");
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if (socket == null)
				return;

			InputStream tmpIn = null;
			OutputStream tmpOut = null;

			try {
				tmpIn = socket.getInputStream();
				tmpOut = socket.getOutputStream();
			} catch (IOException e) {
			}

			istream = tmpIn;
			ostream = tmpOut;

			if (activity.dialog != null && activity.dialog.isShowing())
				activity.dialog.dismiss();
		}
		return;
	}

	public static int readFromTarget() {
		int activity = -1;
		buffer[2] = '1';

		int index = 0;
		byte[] tmp_buff = new byte[1024];
		while (true) {
			// Receive the sync message to ensure pairing with the right device
			int bytes = 0; // bytes returned from read()
			try {
				bytes = istream.read(tmp_buff);

				// Copy to permenant buffer
				for (int i = 0; i < bytes; i++) {
					buffer[index] = tmp_buff[i];
					index++;
				}

				// If the last byte is \0 we are done
				if (buffer[index - 1] == '\0') {
					// Print
					System.out.print("Just read: ");
					for (int i = 0; i < index; i++) {
						System.out.print((char) buffer[i]);
					}
					System.out.println("");
					break;
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return -2;
			}
		}
		activity = Character.digit((char) buffer[0], 10);
		return activity;
	}
}