package com.haofengkeji.hdread;

import java.util.HashMap;

import android.app.Activity;
import android.app.PendingIntent;
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
				break;
			case 4:		// ��Ȩ�޷����豸
				Toast.makeText(MainActivity.this, "44444444444444444", 0).show();
				break;
			case 0x1233:	// ��ʾ��ȡ������Ϣ
				Bundle bundle = msg.getData();
				byte[] byteDeviceData = bundle.getByteArray(READ_DEVICE_BYTE_NAME);
				
				
				//textView.setText(byteDeviceData.toString());
				String str = "";
				for (Byte byte1 : byteDeviceData)
				{
					//System.err.println(byte1);
					//Toast.makeText(MainActivity.this, byte1.toString(), 0).show();
					str += byte1.toString();
					str += ", ";
				}
				textView.setText(null);
				textView.setText(str);
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
				new MyThread2().start();
			}
		});
	}
	
	static {
        System.loadLibrary("HDRead");
    }
	
	public native boolean Start();		// �����豸
	public native void Stop();			// ֹͣ�豸
	
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
				//ȡ���ӵ��豸�ϵ�USB�豸����
				HashMap<String, UsbDevice> map = usbManager.getDeviceList();
				//��������ȡָ����USB�豸
				for (UsbDevice device : map.values())
				{
					Log.e("device", "vid:" + device.getVendorId() + 
							"   pid:" + device.getProductId() + 
							"   " + device.getDeviceName());
					//VendorID �� ProductID  ʮ����
					if (4292 == device.getVendorId() && 1 == device.getProductId())
					{
						usbDevice = device;
					}
				}
				
				//�����Ƿ��в����豸��Ȩ��
				if (usbManager.hasPermission(usbDevice))
				{
					handler.sendEmptyMessage(3);
					new MyThread3().start();
				}
				else
				{
					//û��Ȩ��ѯ���û��Ƿ�����Ȩ��
					handler.sendEmptyMessage(4);
					usbManager.requestPermission(usbDevice, pendingIntent); //�ô���ִ�к�ϵͳ����һ���Ի���
					                                                        //ѯ���û��Ƿ�����������USB�豸��Ȩ��
				}
				
			} 
			catch (Exception e) 
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	// ��д����
	class MyThread3 extends Thread
	{
		@Override
		public void run()
		{
			// TODO Auto-generated method stub
			super.run();
			
			byte[] cmd = new byte[10];
			cmd[0] = 0x01;
			cmd[1] = 0x02;
			cmd[2] = 0x03;
			cmd[3] = 0x04;
			cmd[4] = 0x05;
			cmd[5] = 0x06;
			cmd[6] = 0x07;
			cmd[7] = 0x08;
			cmd[8] = 0x09;
			cmd[9] = 0x20;				
			
			UsbInterface usbInterface = usbDevice.getInterface(0);
			//USBEndpointΪ��д��������Ľڵ�
			UsbEndpoint inEndpoint = usbInterface.getEndpoint(0);  //�����ݽڵ�
			UsbEndpoint outEndpoint = usbInterface.getEndpoint(1); //д���ݽڵ�
			UsbDeviceConnection connection = usbManager.openDevice(usbDevice);
			connection.claimInterface(usbInterface, true);
			
			//��������
			int out = connection.bulkTransfer(outEndpoint, cmd, cmd.length, 3000);
			
			
			while (true)
			{
				//��ȡ����1   ���ַ�����ȡ����
				byte[] byte2 = new byte[16];
				int ret = connection.bulkTransfer(inEndpoint, byte2, byte2.length, 0);
				
				Bundle bundle = new Bundle();
				bundle.putByteArray(READ_DEVICE_BYTE_NAME, byte2);
				Message msg = new Message();
				msg.setData(bundle);
				msg.what = 0x1233;
				handler.sendMessage(msg);
				//Log.e("ret", "ret:" + ret);
				//for (Byte byte1 : byte2)
				//{
					//System.err.println(byte1);
				//}
			}
//			//��ȡ����1   ���ַ�����ȡ����
//			byte[] byte2 = new byte[16];
//			int ret = connection.bulkTransfer(inEndpoint, byte2, byte2.length, 3000);
//			Log.e("ret", "ret:" + ret);
//			for (Byte byte1 : byte2)
//			{
//				System.err.println(byte1);
//			}
			
			//��ȡ����2
			/*int outMax = outEndpoint.getMaxPacketSize();
			int inMax = inEndpoint.getMaxPacketSize();
			ByteBuffer byteBuffer = ByteBuffer.allocate(inMax);
			UsbRequest usbRequest = new UsbRequest();
			usbRequest.initialize(connection, inEndpoint);
			usbRequest.queue(byteBuffer, inMax);
			if(connection.requestWait() == usbRequest){
				byte[] retData = byteBuffer.array();
				for(Byte byte1 : retData){
					System.err.println(byte1);
				}
			}*/
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
	                	handler.sendEmptyMessage(1);
	                    if (usbDevice != null)
	                    {
	                    	new MyThread3().start();
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
