package com.example.swatch;

public interface IBluetoothServiceEventReceiver {


	public void bluetoothEnabling();


	public void bluetoothEnabled();


	public void bluetoothDisabling();


	public void bluetoothDisabled();


	public void connectedTo(final String name, final String address);
}