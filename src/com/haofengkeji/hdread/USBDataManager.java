package com.haofengkeji.hdread;

import java.util.HashMap;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class USBDataManager extends Service {
	private UsbManager usbManager;
	private UsbDevice usbDevice;
	private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
	private PendingIntent pendingIntent;
	public static final Object signal = new Object();
	private byte[] lock = new byte[0];  //�Զ���������
	
	private UsbDeviceConnection connection = null;	// USB����
	private UsbInterface usbInterface = null;		// USB���ӽӿ�
	private UsbEndpoint inEndpoint = null;			// �����ݽڵ�
	private UsbEndpoint outEndpoint = null;			// д���ݽڵ�
	
	private static final String READ_DEVICE_BYTE_NAME = "read_device_data";
	
	// ��ʱ�ص��÷���
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	private Handler handler = new Handler()
	{
		@Override
		public void handleMessage(Message msg) 
		{
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch (msg.what) 
			{
			case 3:		// ��Ȩ�����豸
				//Toast.makeText(USBDataManager.this, "33333333333333333", 0).show();
				startDataCommunication();
				break;
			case 4:		// ��Ȩ�޷����豸
				//Toast.makeText(USBDataManager.this, "44444444444444444", 0).show();
				break;
			}
		}
	};
	
	// Service ������ʱ�ص��÷���
	@Override
	public void onCreate() 
	{
		super.onCreate();

		usbManager = (UsbManager)getSystemService(Context.USB_SERVICE);
		
		pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
		IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
		registerReceiver(mUsbReceiver, filter);
	}
	
	// ͨ�����̣߳��򿪴�������new StartThread().start();��
	class StartThread extends Thread
	{
		@Override
		public void run() 
		{
			// TODO Auto-generated method stub
			super.run();
			StartDevice();
		}
	}
	
	// ����so��
	static {
        System.loadLibrary("HDRead");
    }
	
	public native boolean StartDevice();					// �����豸
	public native void StopDevice();						// ֹͣ�豸
	public native void SetDifficultLevel(int iLevel);		// ���õ�ǰ�����Ѷ�
	public native boolean GetBlockFlag();					// �õ�������־
	public native void SetBlockFlag(boolean bBlockFlag);	// ����������־
	public native void SetReadHRV(boolean bRead);			// HRV�����Ѿ���ȡ
	public native boolean GetReadHRV();						// HRV�Ƿ������ݿ��Զ�ȡ
	public native float[] GetPackHrt();						// ��ȡHRT
	public native int GetIBICount();						// �õ�IBI���ݸ���
	public native int GetArtifactStatus();					// �õ��豸״̬
	
	boolean isOpenDevice = false;

	// �������豸���߳�
	public boolean openDevice() {
		new MyThread2().start();
		synchronized(lock) {			// ͬ��������ȡ�ź���
			try {
				lock.wait();			// �ͷ��ź���
			} catch (InterruptedException e) {
				Log.d("ReadSignal", "timeout");
			}
		}
		return isOpenDevice;
	}
	
	// ������д���ݵ��߳�
	public void startDataCommunication() {
		new Thread(new UpdateThread()).start();
	}
	
	class UpdateThread implements Runnable 
	{
		@Override
		public void run()
		{
			try {
				while (true)
				{
					Thread.sleep(20);
					if ( !GetBlockFlag() )		//�Ƿ���������־
					{
						SetBlockFlag(true);		//����������־
						
						if (GetReadHRV())
						{
							float[] hrv = GetPackHrt();
							int len = hrv.length;
							if (GetIBICount() >= 10)
							{
//										for (int i = 0; i < len; i++)
//											Log.d("hrv", String.valueOf(hrv[i]));
							}
						}
						
						SetReadHRV(false);
			    		SetBlockFlag(false);	//���������־
					}
				}
			}
			catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public byte[] readData()
	{
		int dataLength = 8;
		byte[] bTempReadData = new byte[dataLength];
		int result = connection.bulkTransfer(inEndpoint, bTempReadData, dataLength, 0);
		if (result != -1) {
		}
		return bTempReadData;
	}
	
	// ֹͣ���ݴ���
	public void closeConnection()
	{
		connection.close();
		connection = null;
		usbInterface = null;		// USB���ӽӿ�
		inEndpoint = null;			// �����ݽڵ�
		outEndpoint = null;			// д���ݽڵ�
	}
	
	// �����豸
	private void connectionDevice()
	{
		
	}

	// �����豸
	class MyThread2 extends Thread
	{
		@Override
		public void run() 
		{
			// TODO Auto-generated method stub
			super.run();
			try 
			{
				// ȡ���ӵ��豸�ϵ�USB�豸����
				HashMap<String, UsbDevice> map = usbManager.getDeviceList();
				// ��������ȡָ����USB�豸
				for (UsbDevice device : map.values())
				{
					Log.e("device", "vid:" + device.getVendorId() + 
							"   pid:" + device.getProductId() + 
							"   " + device.getDeviceName());
					// VendorID �� ProductID ʮ����
					if (4292 == device.getVendorId() && 1 == device.getProductId())
					{
						usbDevice = device;
					}
				}
				
				// �����Ƿ��в����豸��Ȩ��
				if (usbManager.hasPermission(usbDevice))
				{
					usbInterface = usbDevice.getInterface(0);
					inEndpoint = usbInterface.getEndpoint(0);  		// �����ݽڵ�
					outEndpoint = usbInterface.getEndpoint(1); 		// д���ݽڵ�
					connection = usbManager.openDevice(usbDevice);
					connection.claimInterface(usbInterface, true);
					
					isOpenDevice = true;
					synchronized(lock) {
						lock.notify();
					}
					
					handler.sendEmptyMessage(3);
				}
				else
				{
					// û��Ȩ��ѯ���û��Ƿ�����Ȩ��
					handler.sendEmptyMessage(4);
					// �����Ի���ѯ���û��Ƿ�����������USB�豸��Ȩ��
					usbManager.requestPermission(usbDevice, pendingIntent);  
				}
				
			} 
			catch (Exception e) 
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() 
	{
	    public void onReceive(Context context, Intent intent) 
	    {
	        String action = intent.getAction();
	        Log.e( "action", action);
	        
	        if (ACTION_USB_PERMISSION.equals(action))
	        {
	            synchronized (this) 
	            {
	                usbDevice = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
	                if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false))
	                {
	                    if (usbDevice != null)
	                    {
	    					usbInterface = usbDevice.getInterface(0);
	    					inEndpoint = usbInterface.getEndpoint(0);  		// �����ݽڵ�
	    					outEndpoint = usbInterface.getEndpoint(1); 		// д���ݽڵ�
	    					connection = usbManager.openDevice(usbDevice);
	    					connection.claimInterface(usbInterface, true);
	    					
	    					isOpenDevice = true;
	    					synchronized(lock) {
	    						lock.notify();
	    					}
	    					
	    					handler.sendEmptyMessage(3);
	                    }
	                } 
	                else 
	                {
	                    Log.d("denied", "permission denied for device " + usbDevice);
	                }
	            }
	        }
	    }
	};
	
}
