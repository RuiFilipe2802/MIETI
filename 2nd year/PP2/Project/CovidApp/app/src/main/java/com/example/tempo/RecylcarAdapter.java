package com.example.tempo;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

class RecylcarAdapter extends RecyclerView.Adapter<RecylcarAdapter.ViewHolder> {

    private ArrayList<String> nomesCid;
    private OnItemClickListener mListener;

    public interface OnItemClickListener{
        void onItemClick(int position);
    }

    public void setOnItemClickListener(OnItemClickListener listener){
            mListener = listener;
    }

    public RecylcarAdapter(ArrayList<String> nomesCid) {
        this.nomesCid = nomesCid;
    }

    @Override
    public RecylcarAdapter.ViewHolder onCreateViewHolder( ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.linha, parent, false);
        return new ViewHolder(v);
    }

    @Override
    public void onBindViewHolder(RecylcarAdapter.ViewHolder holder, int position) {
        holder.nom.setText(nomesCid.get(position));
    }

    @Override
    public int getItemCount() {

        return nomesCid.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {

        public TextView nom;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            nom = itemView.findViewById(R.id.textView_linha_recycler);

            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                   if(mListener!= null){
                       int position = getAdapterPosition();
                       if(position != RecyclerView.NO_POSITION){
                           mListener.onItemClick(position);
                       }
                   }
                }
            });
        }
    }
}
