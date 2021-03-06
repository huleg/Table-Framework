package com.remote.btcontroller;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.os.AsyncTask;
import android.os.Handler;
import android.content.Intent;
import android.widget.Button;
import android.widget.ToggleButton;
import android.widget.CompoundButton;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.HapticFeedbackConstants;
import android.view.MotionEvent;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import java.io.IOException;
import java.util.UUID;
import java.util.Calendar;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public class ControllerActivity extends AppCompatActivity implements OnTouchListener
{
    // Initialization
    Button btn_up;
    Button btn_down;
    Button btn_left;
    Button btn_right;
    Button btn_a;
    Button btn_b;
    Button btn_start;
    Button btn_select;
    ToggleButton btn_clock;

    String date    = null;
    String time    = null;
    DateFormat df1 = new SimpleDateFormat("MM-dd");
    DateFormat df2 = new SimpleDateFormat("hh:mm");

    Toast message;
    private ProgressDialog progress;
    String address = null;
    BluetoothAdapter mBluetooth   = null;
    BluetoothSocket btSocket      = null;
    private boolean isBtConnected = false;
    static final UUID mUUID       = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // Receive the address of the bluetooth device
        Intent newIntent = getIntent();
        address = newIntent.getStringExtra(MainActivity.EXTRA_ADDRESS);

        setContentView(R.layout.activity_bt_controller);

        // Call the buttons
        btn_up     = (Button)findViewById(R.id.btn_up);
        btn_down   = (Button)findViewById(R.id.btn_down);
        btn_left   = (Button)findViewById(R.id.btn_right);
        btn_right  = (Button)findViewById(R.id.btn_left);
        btn_a      = (Button)findViewById(R.id.btn_a);
        btn_b      = (Button)findViewById(R.id.btn_b);
        btn_start  = (Button)findViewById(R.id.btn_start);
        btn_select = (Button)findViewById(R.id.btn_select);
        btn_clock  = (ToggleButton)findViewById(R.id.btn_clock);

        // Call the class to connect
        new btConnect().execute();

        btn_up.setOnTouchListener(this);
        btn_down.setOnTouchListener(this);
        btn_left.setOnTouchListener(this);
        btn_right.setOnTouchListener(this);
        btn_a.setOnTouchListener(this);
        btn_b.setOnTouchListener(this);
        btn_start.setOnTouchListener(this);
        btn_select.setOnTouchListener(this);

        btn_clock.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
        {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
                if (isChecked)
                {
                    handler.post(clockUpdater);
                }

                else
                {
                    handler.removeCallbacks(clockUpdater);
                }
            }
        });
    }

    private Handler handler       = new Handler();
    private Runnable clockUpdater = new Runnable()
    {
        @Override
        public void run()
        {
            refreshClock();
            handler.postDelayed(this, 1000);
        }
    };

    @Override
    public boolean onTouch(View view, MotionEvent event)
    {
        if (event.getAction() == MotionEvent.ACTION_DOWN)
        {
            view.performHapticFeedback(HapticFeedbackConstants.VIRTUAL_KEY);

            switch (view.getId())
            {
                case R.id.btn_up:
                    btWrite("w\0");
                    break;

                case R.id.btn_down:
                    btWrite("s\0");
                    break;

                case R.id.btn_left:
                    btWrite("a\0");
                    break;

                case R.id.btn_right:
                    btWrite("d\0");
                    break;

                case R.id.btn_a:
                    btWrite("q\0");
                    break;

                case R.id.btn_b:
                    btWrite("e\0");
                    break;

                case R.id.btn_start:
                    btWrite("h\0");
                    break;

                case R.id.btn_select:
                    btWrite("j\0");
                    break;
            }
        }

        // If the button is released or dragged out of view
        if ((event.getAction() == MotionEvent.ACTION_UP) ||
                (event.getAction() == MotionEvent.ACTION_CANCEL))
        {
            switch (view.getId())
            {
                case R.id.btn_up:
                    btWrite("W\0");
                    break;

                case R.id.btn_down:
                    btWrite("S\0");
                    break;

                case R.id.btn_left:
                    btWrite("A\0");
                    break;

                case R.id.btn_right:
                    btWrite("D\0");
                    break;

                case R.id.btn_a:
                    btWrite("Q\0");
                    break;

                case R.id.btn_b:
                    btWrite("E\0");
                    break;

                case R.id.btn_start:
                    btWrite("H\0");
                    break;

                case R.id.btn_select:
                    btWrite("J\0");
                    break;
            }
        }

        return true;
    }

    // format and refresh the time and date strings
    private void refreshClock()
    {
        date = df1.format(Calendar.getInstance().getTime());
        time = df2.format(Calendar.getInstance().getTime());
        btWrite("z");
        btWrite(date + "\0");
        btWrite(time + "\0");
    }

    // Make a toast notification
    private void msg(String s)
    {
        if (message != null)
        {
            message.cancel();
        }

        message = Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT);
        message.show();
    }

    @Override
    public void onBackPressed()
    {
        handler.removeCallbacks(clockUpdater);
        btDisconnect();
        super.onBackPressed();
    }

    private void btWrite(String m)
    {
        // If the btSocket is busy
        if (btSocket != null)
        {
            try
            {
                // Write to the connected Bluetooth device
                btSocket.getOutputStream().write(m.toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void btDisconnect()
    {
        // If the btSocket is busy
        if (btSocket != null)
        {
            try
            {
                // Close the connection
                msg("Disconnected");
                btSocket.close();
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }

        finish();
    }

    private class btConnect extends AsyncTask<Void, Void, Void>
    {
        // Almost connected
        private boolean ConnectSuccess = true;

        @Override
        protected void onPreExecute()
        {
            // Show progress dialog
            progress = ProgressDialog.show(ControllerActivity.this, "Please Wait",
                    "Connecting to device...");
        }

        @Override
        protected Void doInBackground(Void ... devices)
        {
            // While progress dialog is shown, connection is done in background
            try
            {
                // If the btSocket is not busy or no device is connected
                if ((btSocket == null) || !isBtConnected)
                {
                    // Get the mobile bluetooth device
                    mBluetooth = BluetoothAdapter.getDefaultAdapter();
                    // Connect to the device address and checks if available
                    BluetoothDevice device = mBluetooth.getRemoteDevice(address);
                    // Create an SPP connection
                    btSocket = device.createInsecureRfcommSocketToServiceRecord(mUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    // Start the connection
                    btSocket.connect();
                }
            }

            catch (IOException e)
            {
                // If connection failed, check the exception
                ConnectSuccess = false;
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void result)
        {
            // Check if everything worked after connecting in the background
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg("Connection failed, try again");
                finish();
            }

            else
            {
                msg("Connected");
                isBtConnected = true;
            }

            progress.dismiss();
        }
    }
}
