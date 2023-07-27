package com.example.tempo;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class SecondActivity extends AppCompatActivity {

    RecyclerView mRecyclerView;
    RecyclerView.LayoutManager mLayoutManager;
    RecylcarAdapter mAdapter;
    HashMap<Integer,String>idCids;
    ArrayList<String> array;
    FloatingActionButton fl;
    int id;
    HashMap<Integer, String> idCids3;

    @SuppressLint("WrongViewCast")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second);

        Bundle b = getIntent().getExtras();
        idCids = (HashMap<Integer,String>) b.getSerializable("ArrayCids");
        array = new ArrayList<String>(idCids.values());
        idCids3 = new HashMap<>();
        mRecyclerView = findViewById(R.id.recycler_cidade);
        mRecyclerView.setHasFixedSize(true);
        mLayoutManager = new LinearLayoutManager(this);
        mAdapter = new RecylcarAdapter(array);
        mRecyclerView.setLayoutManager(mLayoutManager);
        mRecyclerView.setAdapter(mAdapter);

        mAdapter.setOnItemClickListener(new RecylcarAdapter.OnItemClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.KITKAT)
            @Override
            public void onItemClick(int position) {
                //array3.add(array.get(position));

                for(Map.Entry<Integer,String> e : idCids.entrySet()) {
                    if (Objects.equals(array.get(position), e.getValue())) {
                         idCids3.put(e.getKey(), array.get(position));
                    }
                }
                Toast.makeText(SecondActivity.this, "Cidade Adicionada", Toast.LENGTH_SHORT).show();
            }
        });

        fl = findViewById(R.id.butoon_flo);

        fl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openThirdAct();
            }
        });
    }

    public void openThirdAct() {
        Intent intent = new Intent(this, ThirdActivity.class);
        Bundle bundle = new Bundle();
        bundle.putSerializable("ArrayCid", idCids3);
        intent.putExtras(bundle);
        startActivity(intent);
    }
}