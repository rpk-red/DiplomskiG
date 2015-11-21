package com.example.laptopg.my1stapplication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {

    private static final int REQUEST_ENABLE_BT = 1;
    private static final UUID MY_UUID = UUID.fromString("f00001101-0000-1000-8000-00805F9B34FB");
    private static final String LOG_TAG = "MainActivity";
    public BluetoothAdapter adapterBT;
    private ListView listView;
    private ArrayAdapter<String> listDevices;
    private Set<BluetoothDevice> pairedDevices;
    private BroadcastReceiver mReciver;
    private IntentFilter mFilter;
    BluetoothDevice device;
    private BluetoothSocket mySocket = null;
    ConnectThread myConnectThread;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(LOG_TAG, "OnCreate");
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        checkBT();
        discoverBT();
        init();


    }


    private void init() {

        Button napredBtn = (Button) findViewById(R.id.btnNapred);
        Button nazadBtn = (Button) findViewById(R.id.btnNazad);
        Button levoBtn = (Button) findViewById(R.id.btnLevo);
        Button desnoBtn = (Button) findViewById(R.id.btnDesno);

        listView = (ListView) findViewById(R.id.listView);
        listDevices = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
        listView.setAdapter(listDevices);
        listView.setOnItemClickListener(this);
        pairDevices();
        mFilter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        mReciver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (BluetoothDevice.ACTION_FOUND.equals(action)){
                    device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    listDevices.add(device.getName() + "\n" + device.getAddress());
            }
            }
        };
        registerReceiver(mReciver, mFilter);
    }



    private void discoverBT() {
        adapterBT.cancelDiscovery();
        adapterBT.startDiscovery();
    }

    private void checkBT() {

        adapterBT = BluetoothAdapter.getDefaultAdapter();

        if(adapterBT == null)
        {
            Toast.makeText(this,"Bluetooth not found!",Toast.LENGTH_SHORT).show();
        }
        if(!adapterBT.isEnabled())
        {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent,REQUEST_ENABLE_BT);
        }
    }


    private void pairDevices() {
        pairedDevices = adapterBT.getBondedDevices();
        if(pairedDevices.size()>0)
        {
            for(BluetoothDevice device : pairedDevices)
                listDevices.add(device.getName()+ "\n" + device.getAddress());
        }
    }
    private void unpairDevices(){

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void onClickHandler(View view) {
        if (view.getId() == R.id.btnNapred){
            String mString = "NAPRED";
            byte[] b = mString.getBytes(Charset.forName("UTF-8"));
            myConnectThread.write(b);
        }
        else if (view.getId() == R.id.btnNazad){
            String mString = "NAZAD";
            byte[] b = mString.getBytes(Charset.forName("UTF-8"));
            myConnectThread.write(b);
        }
        else if (view.getId() == R.id.btnLevo){
            String mString = "LEVO";
            byte[] b = mString.getBytes(Charset.forName("UTF-8"));
            myConnectThread.write(b);
        }
        else if (view.getId() == R.id.btnDesno){
            String mString = "DESNO";
            byte[] b = mString.getBytes(Charset.forName("UTF-8"));
            myConnectThread.write(b);
        }
        else {
            unpairDevices();
        }

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == RESULT_CANCELED)
        {
            Toast.makeText(this,"Bluetooth must be enabled!",Toast.LENGTH_SHORT).show();
            finish();
        }
    }


        @Override
        public void onItemClick (AdapterView < ? > parent, View view,int position, long id){

//            Log.d(LOG_TAG, "Kliknut");
            String[] itemSelected =  listDevices.getItem(position).split("\n");
            Toast.makeText(this, "Name: " + itemSelected[0] + "\n" + "Adress:" + itemSelected[1],Toast.LENGTH_LONG).show();
            if (adapterBT.checkBluetoothAddress(itemSelected[1])== true)
                device = adapterBT.getRemoteDevice(itemSelected[1]);
            myConnectThread = new ConnectThread(device);
            myConnectThread.run();

        }

    private class ConnectThread extends Thread{

        BluetoothSocket tmp_socket;
        BluetoothDevice tmp_device;
        private  InputStream mmInputStream;
        private  OutputStream mmOutputStream;

        public ConnectThread(BluetoothDevice device){

            tmp_socket = null;
            tmp_device = device;
            mmInputStream = null;
            mmOutputStream = null;

            try {
                tmp_socket = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {}

            mySocket = tmp_socket;

        }
        public void run() {

            adapterBT.cancelDiscovery();

            try {
                mySocket.connect();

            } catch (IOException connectException) {

                try {
                    mySocket.close();
                } catch (IOException closeException) {
                }
            }

            transferData(mySocket);
        }

        public void cancel(){
            try {
                mySocket.close();
            }catch (IOException e) {}
        }


        public void write(byte[] bytes){
            try {
                mmOutputStream.write(bytes);
            }catch (IOException e){}
        }
        public void read(byte[] bytes) {
            try {
                mmInputStream.read(bytes);
            } catch (IOException e) {}
        }

        private void transferData(BluetoothSocket mySocket) {

        }

    }


}



