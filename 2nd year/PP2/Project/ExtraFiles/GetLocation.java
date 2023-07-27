package com.example.covidapp;

import android.annotation.SuppressLint;
import android.content.Context;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.os.AsyncTask;
import android.util.Log;

import java.io.IOException;
import java.util.List;
import java.util.Locale;

public class GetLocation extends AsyncTask<Location, Void, String> {

        private final String TAG = "QWERTY";
        @SuppressLint("StaticFieldLeak")
        private Context mContext;
        private OnTaskCompleted mListener;

        GetLocation(Context applicationContext, OnTaskCompleted listener) {
            mContext = applicationContext;
            mListener = listener;
        }

        @Override
        protected String doInBackground(Location... params) {
            // Set up the geocoder
            Geocoder geocoder = new Geocoder(mContext, Locale.getDefault());
            // Get the passed in location
            Location location = params[0];
            List<Address> morada = null;
            String resultMessage = "";
            String resultMessage2 = "";
            try {
                morada = geocoder.getFromLocation(location.getLatitude(), location.getLongitude(),1);
            } catch (IOException ioException) {
                // Catch network or other I/O problems
                resultMessage = mContext.getString(R.string.service_not_available);
                Log.e(TAG, resultMessage, ioException);
            } catch (IllegalArgumentException illegalArgumentException) {
                // Catch invalid latitude or longitude values
                resultMessage = mContext.getString(R.string.invalid_lat_long_used);
                Log.e(TAG,resultMessage + ". " + "Latitude = " + location.getLatitude() + ", Longitude = " +
                        location.getLongitude(), illegalArgumentException);
            }

            if (morada == null || morada.size() == 0) {
                if (resultMessage.isEmpty()) {
                    resultMessage = mContext.getString(R.string.no_address_found);
                }
            }else {
                Address address = morada.get(0);
                resultMessage = address.getLocality();
                resultMessage2 = address.getLatitude()+"_"+address.getLongitude();
            }
            return resultMessage+","+resultMessage2;
        }

        @Override
        protected void onPostExecute(String address) {
            mListener.onTaskCompleted(address);
            super.onPostExecute(address);
        }

        interface OnTaskCompleted {
            void onTaskCompleted(String result);
        }
    }
