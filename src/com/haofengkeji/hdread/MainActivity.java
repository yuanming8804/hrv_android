package com.haofengkeji.hdread;

import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity 
{
	private UsbManager usbManager;
	private UsbDevice usbDevice;
	private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
	private PendingIntent pendingIntent;
	public static final Object signal = new Object();
	private byte[] lock = new byte[0];  //�Զ���������
	
	private static final String READ_DEVICE_BYTE_NAME = "read_device_data";
	TextView textView;
	
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
				Toast.makeText(MainActivity.this, "33333333333333333", 0).show();

				startDataCommunication();
				break;
			case 4:		// ��Ȩ�޷����豸
				Toast.makeText(MainActivity.this, "44444444444444444", 0).show();
				break;
			case 5:
				//synchronized(signal) {
				//int result = connection.bulkTransfer(inEndpoint, bReadData, dataLength, 0);
				//	if (result != -1)
				//		signal.notify();
				//}
				
				break;
			case 6:
//				isOpenDevice = true;
				
//				if ( !GetBlockFlag() )		//�Ƿ���������־
//				{
//					SetBlockFlag(true);		//����������־
//					GetIBICount();
//		    		SetBlockFlag(false);	//���������־
//				}
				Bundle bundle1 = msg.getData();
				int i = bundle1.getInt(READ_DEVICE_BYTE_NAME);
				textView.setText(null);
				String str = String.valueOf(i);
				textView.setText(str);
				break;
			case 0x1233:	// ��ʾ��ȡ������Ϣ
//				Bundle bundle = msg.getData();
//				byte[] byteDeviceData = bundle.getByteArray(READ_DEVICE_BYTE_NAME);
				
				
				//textView.setText(byteDeviceData.toString());
//				String str = "";
//				for (Byte byte1 : byteDeviceData)
//				{
//					//System.err.println(byte1);
//					//Toast.makeText(MainActivity.this, byte1.toString(), 0).show();
//					str += byte1.toString();
//					str += ", ";
//				}
//				textView.setText(null);
//				textView.setText(str);
				break;
			}
		}
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		textView = (TextView)findViewById(R.id.textView1);
		//Start();		
		usbManager = (UsbManager)getSystemService(Context.USB_SERVICE);
		
		pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
		IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
		registerReceiver(mUsbReceiver, filter);
		
		Button btnConnect = (Button)findViewById(R.id.btnConnect);
		btnConnect.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				new StartThread().start();
				//StartDevice();
			}
		});
		
		Button btnTest = (Button)findViewById(R.id.btnTest);
		final TextView txtTest = (TextView)findViewById(R.id.txtTest);
		btnTest.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				txtTest.setText(String.valueOf(iTest++));
			}
		});
	}
	private static int iTest = 0;
	
	// �򿪴��������߳�
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
	
	
	static {
        System.loadLibrary("HDRead");
    }
	
	public enum E_DIFFICULTLEVEL {
		eLevel_Easy,
		eLevel_Normal,
		eLevel_Hard,
		eLevel_Heaven,
		
		eLevel_Count
	};
	
	public native boolean StartDevice();					// �����豸
	public native void StopDevice();						// ֹͣ�豸
	public native void SetDifficultLevel(int iLevel);		// ���õ�ǰ�����Ѷ�
	public native boolean GetBlockFlag();					// �õ�������־
	public native void SetBlockFlag(boolean bBlockFlag);	// ����������־
	public native void SetReadHRV(boolean bRead);			// HRV�����Ѿ���ȡ
	public native boolean GetReadHRV();						// HRV�Ƿ������ݿ��Զ�ȡ
	public native float[] GetPackHrt();
	public native int GetIBICount();						// �õ�IBI���ݸ���
	public native int GetArtifactStatus();					// �õ��豸״̬
	
	boolean isOpenDevice = false;
	boolean condition = false;
	// �������豸���߳�
	public boolean openDevice() {
		new MyThread2().start();
		//while(!condition) {
			synchronized(lock) {			// ͬ��������ȡ�ź���
				try {
					lock.wait();			// �ͷ��ź���
				} catch (InterruptedException e) {
					Log.d("ReadSignal", "timeout");
				}
			}
		//}
		return isOpenDevice;
	}
	
	// ������д���ݵ��߳�
	public void startDataCommunication() {
		//new MyThread3().start();
//		usbInterface = usbDevice.getInterface(0);
//		// USBEndpointΪ��д��������Ľڵ�
//		inEndpoint = usbInterface.getEndpoint(0);  		// �����ݽڵ�
//		outEndpoint = usbInterface.getEndpoint(1); 		// д���ݽڵ�
//		connection = usbManager.openDevice(usbDevice);
//		connection.claimInterface(usbInterface, true);
		
		//timer.schedule(task, 20, 20);
		new Thread(new UpdateThread()).start();
	}
	//private static int iTest1 = 0;
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
						
						Bundle bundle = new Bundle();
						bundle.putInt(READ_DEVICE_BYTE_NAME, GetIBICount());
						//bundle.putInt(READ_DEVICE_BYTE_NAME, ++iTest1); 
						Message msg = new Message();
						msg.setData(bundle);
						msg.what = 6;
						handler.sendMessage(msg);
						
						if (GetReadHRV())
						{
							float[] hrv = GetPackHrt();
							int len = hrv.length;
							if (GetIBICount() >= 10)
							{
//								for (int i = 0; i < len; i++)
//									Log.d("hrv", String.valueOf(hrv[i]));
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
	
//	private final Timer timer = new Timer();
//	TimerTask task = new TimerTask() {
//	    @Override
//	    public void run() {
//	        // TODO Auto-generated method stub
//	        Message message = new Message();
//	        message.what = 6;
//	        handler.sendMessage(message);
//	    	
//	    }
//	};
	
//	Handler handler = new Handler() {
//	    @Override
//	    public void handleMessage(Message msg) {
//	        // TODO Auto-generated method stub
//	        // Ҫ��������
//	        super.handleMessage(msg);
//	    }
//	};
	
	//int dataLength = 16;
	//byte[] bReadData = new byte[dataLength];	// ��ȡ����USB����
	// ��ȡ����
	public byte[] readData()
	{
//		Bundle bundle = new Bundle();
//		bundle.putByteArray(READ_DEVICE_BYTE_NAME, bReadData);
//		Message msg = new Message();
//		msg.setData(bundle);
//		msg.what = 0x1233;
//		handler.sendMessage(msg);
		
		//handler.sendEmptyMessage(5);	// ֪ͨ���̶߳�ȡUSB����
		
		int dataLength = 8;
		byte[] bTempReadData = new byte[dataLength];
		
		//int result = connection.controlTransfer(UsbConstants.USB_DIR_IN | UsbConstants.USB_ENDPOINT_XFER_INT, request, value, index, buffer, length, timeout)
		int result = connection.bulkTransfer(inEndpoint, bTempReadData, dataLength, 0);
		
		//int aaa = inEndpoint.getType();
//		for (int index = 0; index < bTempReadData.length; index++) 
//		{
//			int aa = bTempReadData[index];
//			Log.i("myactivity", String.format("%d", aa));
//		}
//		Log.i("myactivity", "============================================");
//		for (int i = bTempReadData.length - 1; i > 0; i--)
//		{
//			bTempReadData[i] = bTempReadData[i - 1];
//			Log.i("myactivity", String.format("0x%20x", bTempReadData[i]));
//		}
//		bTempReadData[0] = 0;
		if (result != -1) {
			
		}
//		synchronized(signal) {			// ͬ��������ȡ�ź���
//			try {
//				signal.wait();			// �ͷ��ź���
				//bTempReadData = bReadData.clone();
//			}
//			catch (InterruptedException e) {
//				Log.d("ReadSignal", "timeout");
//			}
//		}

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
					// USBEndpointΪ��д��������Ľڵ�
					inEndpoint = usbInterface.getEndpoint(0);  		// �����ݽڵ�
					outEndpoint = usbInterface.getEndpoint(1); 		// д���ݽڵ�
					connection = usbManager.openDevice(usbDevice);
					connection.claimInterface(usbInterface, true);
					
					//int count = usbInterface.getEndpointCount();
					
					isOpenDevice = true;
					//condition = true;
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
	
	
	UsbDeviceConnection connection = null;	// USB����
	UsbInterface usbInterface = null;		// USB���ӽӿ�
	UsbEndpoint inEndpoint = null;			// �����ݽڵ�
	UsbEndpoint outEndpoint = null;			// д���ݽڵ�
	
//	// ��д����
//	class MyThread3 extends Thread
//	{
//		@Override
//		public void run()
//		{
//			// TODO Auto-generated method stub
//			super.run();
//			
//			byte[] cmd = new byte[10];
//			cmd[0] = 0x01;
//			cmd[1] = 0x02;
//			cmd[2] = 0x03;
//			cmd[3] = 0x04;
//			cmd[4] = 0x05;
//			cmd[5] = 0x06;
//			cmd[6] = 0x07;
//			cmd[7] = 0x08;
//			cmd[8] = 0x09;
//			cmd[9] = 0x20;
//			
//			usbInterface = usbDevice.getInterface(0);
//			// USBEndpointΪ��д��������Ľڵ�
//			inEndpoint = usbInterface.getEndpoint(0);  // �����ݽڵ�
//			outEndpoint = usbInterface.getEndpoint(1); // д���ݽڵ�
//			connection = usbManager.openDevice(usbDevice);
//			connection.claimInterface(usbInterface, true);
//			
//			// ��������
//			int out = connection.bulkTransfer(outEndpoint, cmd, cmd.length, 3000);
//			
//			while (true)
//			{
//				// ��ȡ����1   ���ַ�����ȡ����
//				byte[] byte2 = new byte[16];
//				for (Byte byte1 : byte2)
//				{
//					byte1 = 0;
//				}
//				int ret = connection.bulkTransfer(inEndpoint, byte2, byte2.length, 0);
//				
//				Bundle bundle = new Bundle();
//				bundle.putByteArray(READ_DEVICE_BYTE_NAME, byte2);
//				Message msg = new Message();
//				msg.setData(bundle);
//				msg.what = 0x1233;
//				handler.sendMessage(msg);
//				
//				//SetInputReport(byte2);
//				//Log.e("ret", "ret:" + ret);
//				//for (Byte byte1 : byte2)
//				//{
//					//System.err.println(byte1);
//				//}
//			}
////			// ��ȡ����1   ���ַ�����ȡ����
////			byte[] byte2 = new byte[16];
////			int ret = connection.bulkTransfer(inEndpoint, byte2, byte2.length, 3000);
////			Log.e("ret", "ret:" + ret);
////			for (Byte byte1 : byte2)
////			{
////				System.err.println(byte1);
////			}
//			
//			// ��ȡ����2
//			/*int outMax = outEndpoint.getMaxPacketSize();
//			int inMax = inEndpoint.getMaxPacketSize();
//			ByteBuffer byteBuffer = ByteBuffer.allocate(inMax);
//			UsbRequest usbRequest = new UsbRequest();
//			usbRequest.initialize(connection, inEndpoint);
//			usbRequest.queue(byteBuffer, inMax);
//			if(connection.requestWait() == usbRequest){
//				byte[] retData = byteBuffer.array();
//				for(Byte byte1 : retData){
//					System.err.println(byte1);
//				}
//			}*/
//		}
//	}		
	
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
	                	handler.sendEmptyMessage(1);
	                    if (usbDevice != null)
	                    {
	                    	//new MyThread3().start();
	    					usbInterface = usbDevice.getInterface(0);
	    					// USBEndpointΪ��д��������Ľڵ�
	    					inEndpoint = usbInterface.getEndpoint(0);  		// �����ݽڵ�
	    					outEndpoint = usbInterface.getEndpoint(1); 		// д���ݽڵ�
	    					connection = usbManager.openDevice(usbDevice);
	    					connection.claimInterface(usbInterface, true);
	    					
	    					int count = usbInterface.getEndpointCount();
	    					
	    					isOpenDevice = true;
	    					//condition = true;
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
