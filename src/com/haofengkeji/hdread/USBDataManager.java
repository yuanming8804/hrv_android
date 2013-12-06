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
	private byte[] lock = new byte[0];  //自定义锁对象
	
	private UsbDeviceConnection connection = null;	// USB连接
	private UsbInterface usbInterface = null;		// USB连接接口
	private UsbEndpoint inEndpoint = null;			// 读数据节点
	private UsbEndpoint outEndpoint = null;			// 写数据节点
	
	private static final String READ_DEVICE_BYTE_NAME = "read_device_data";
	
	// 绑定时回调该方法
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
			case 3:		// 有权访问设备
				//Toast.makeText(USBDataManager.this, "33333333333333333", 0).show();
				startDataCommunication();
				break;
			case 4:		// 无权限访问设备
				//Toast.makeText(USBDataManager.this, "44444444444444444", 0).show();
				break;
			}
		}
	};
	
	// Service 被创建时回调该方法
	@Override
	public void onCreate() 
	{
		super.onCreate();

		usbManager = (UsbManager)getSystemService(Context.USB_SERVICE);
		
		pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
		IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
		registerReceiver(mUsbReceiver, filter);
	}
	
	// 通过此线程，打开传感器（new StartThread().start();）
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
	
	// 本地so库
	static {
        System.loadLibrary("HDRead");
    }
	
	public native boolean StartDevice();					// 启动设备
	public native void StopDevice();						// 停止设备
	public native void SetDifficultLevel(int iLevel);		// 设置当前测量难度
	public native boolean GetBlockFlag();					// 得到阻塞标志
	public native void SetBlockFlag(boolean bBlockFlag);	// 设置阻塞标志
	public native void SetReadHRV(boolean bRead);			// HRV数据已经读取
	public native boolean GetReadHRV();						// HRV是否有数据可以读取
	public native float[] GetPackHrt();						// 获取HRT
	public native int GetIBICount();						// 得到IBI数据个数
	public native int GetArtifactStatus();					// 得到设备状态
	
	boolean isOpenDevice = false;

	// 开启打开设备的线程
	public boolean openDevice() {
		new MyThread2().start();
		synchronized(lock) {			// 同步锁，获取信号量
			try {
				lock.wait();			// 释放信号量
			} catch (InterruptedException e) {
				Log.d("ReadSignal", "timeout");
			}
		}
		return isOpenDevice;
	}
	
	// 开启读写数据的线程
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
					if ( !GetBlockFlag() )		//是否有阻塞标志
					{
						SetBlockFlag(true);		//设置阻塞标志
						
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
			    		SetBlockFlag(false);	//清除阻塞标志
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
	
	// 停止数据传输
	public void closeConnection()
	{
		connection.close();
		connection = null;
		usbInterface = null;		// USB连接接口
		inEndpoint = null;			// 读数据节点
		outEndpoint = null;			// 写数据节点
	}
	
	// 连接设备
	private void connectionDevice()
	{
		
	}

	// 连接设备
	class MyThread2 extends Thread
	{
		@Override
		public void run() 
		{
			// TODO Auto-generated method stub
			super.run();
			try 
			{
				// 取连接到设备上的USB设备集合
				HashMap<String, UsbDevice> map = usbManager.getDeviceList();
				// 遍历集合取指定的USB设备
				for (UsbDevice device : map.values())
				{
					Log.e("device", "vid:" + device.getVendorId() + 
							"   pid:" + device.getProductId() + 
							"   " + device.getDeviceName());
					// VendorID 和 ProductID 十进制
					if (4292 == device.getVendorId() && 1 == device.getProductId())
					{
						usbDevice = device;
					}
				}
				
				// 程序是否有操作设备的权限
				if (usbManager.hasPermission(usbDevice))
				{
					usbInterface = usbDevice.getInterface(0);
					inEndpoint = usbInterface.getEndpoint(0);  		// 读数据节点
					outEndpoint = usbInterface.getEndpoint(1); 		// 写数据节点
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
					// 没有权限询问用户是否授予权限
					handler.sendEmptyMessage(4);
					// 弹出对话框，询问用户是否授予程序操作USB设备的权限
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
	    					inEndpoint = usbInterface.getEndpoint(0);  		// 读数据节点
	    					outEndpoint = usbInterface.getEndpoint(1); 		// 写数据节点
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
