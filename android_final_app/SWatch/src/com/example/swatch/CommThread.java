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

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.util.Log;


class CommThread extends Thread {
    private static BluetoothSocket socket;
    private InputStream istream;
    private static OutputStream ostream;
    private Handler handler;
    private ProgressDialog dialog;
    private BluetoothAdapter adapter;
    private byte[] buffer = new byte[1024];
    

    public CommThread(BluetoothAdapter adapter, ProgressDialog dialog, Handler handler) {
        this.handler = handler;
        this.dialog = dialog;
        this.adapter = adapter;
    }


    public void run() {
                if (adapter == null)
                        return;

                
                
                
                /*buffer[1] = '1';
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();
                handler.obtainMessage(b, 3, -1, buffer)
                .sendToTarget();*/
                
    
                Set<BluetoothDevice> devices = adapter.getBondedDevices();
                BluetoothDevice device = null;
                for (BluetoothDevice curDevice : devices) {
                        if (curDevice.getName().matches("RN42-CD0C")) {
                                device = curDevice;
                                System.out.println("Device found. Breaking!");
                                break;
                        }
                }
                if (device == null)
                {
                        device = adapter.getRemoteDevice("00:06:66:03:A7:52");
                        System.out.println("Device not found");
                }

                try 
                {
                	Method m = null;
					try {
						m = device.getClass().getMethod("createRfcommSocket", new Class[] {int.class});
					} catch (NoSuchMethodException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
                    socket = (BluetoothSocket) m.invoke(device, 1);
                	
                        //socket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
                        socket.connect();
                } catch (IOException e) 
                {
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
                if (socket == null) return;


        InputStream tmpIn = null;
        OutputStream tmpOut = null;


        try {
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) { }


        istream = tmpIn;
        ostream = tmpOut;
        
        if (dialog != null && dialog.isShowing())
                dialog.dismiss();
     try {
		Thread.sleep(10000);
	} catch (InterruptedException e1) {
		// TODO Auto-generated catch block
		e1.printStackTrace();
	}
     // testing handler
       /* buffer[0] = '2';
        buffer[1] = '0';
        buffer[2] = '\0';
       
        int b = Character.digit((char) buffer[0], 10);
        System.out.println("INT "+b);
        handler.obtainMessage(b, 3, -1, buffer)
        .sendToTarget();
        try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        buffer[1] = '1';
        handler.obtainMessage(b, 3, -1, buffer)
        .sendToTarget();*/
        int act = -1;
        // Read from Bluetooth always
        while (true) {
        	System.out.println("in while");
             	
        	act = readFromTarget();
        	handler.obtainMessage(act, buffer)
            .sendToTarget();
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
        } catch (IOException e) { }
    }
    
    public int readFromTarget() {
		int activity = -1;
		buffer[2] = '1';
		
		int index = 0;
		byte[] tmp_buff = new byte[1024];
		while(true) {
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
				if(buffer[index - 1] == '\0') {
					// Print
					System.out.print("Just read: ");
					for(int i = 0; i < index; i++) {
						System.out.print((char) buffer[i]);
					}
					System.out.println("");
					break;
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		activity = Character.digit((char) buffer[0], 10);
		
//		while(true) 
//		{
//			// Receive the sync message to ensure pairing with the right device
//			int bytes = 0; // bytes returned from read()
//			try 
//			{
//				bytes = istream.read(tmp_buff);
//				System.out.println("BYTE LENGTH "+bytes);
//				System.out.println("BYTES RCVD: "+(char)tmp_buff[0]+(char)tmp_buff[1]+(char)tmp_buff[2]);
//				// Copy to permenant buffer
//				if (bytes == 3)
//				{
//					for (int i = 0; i < 3; i++) 
//					{
//						buffer[i] = tmp_buff[i];
//						System.out.println("buffer "+i+" "+(char)buffer[i]);
//					}
//					activity = Character.digit((char) buffer[0], 10);
//					tmp_buff = null;
//					break;
//				}
//				
//				// Send the obtained bytes to the UI Activity
//				// headlines = 0, weather = 1, time = 2 and buff[1] = 0
//				
//				
//				
//				// If the last byte is \0 we are done
//				/*if(buffer[2] == '\0') 
//				{
//					System.out.println("Sending to target "+(char)buffer[0]+(char)buffer[1]+(char)buffer[2]);
//					tmp_buff = null;
//					break;
//				}*/
//			
//			} 
//			catch (IOException e) 
//			{
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
//		}
		return activity;
	}
}