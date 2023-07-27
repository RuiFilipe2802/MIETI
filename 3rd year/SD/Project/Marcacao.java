import java.util.Date;

public class Marcacao {

    private int nSns;
    private String local;
    private Date hora_marcacao;
    private String estado;

    public Marcacao(int nSns, String local, Date hora_marcacao, String estado) {
        this.nSns = nSns;
        this.local = local;
        this.hora_marcacao = hora_marcacao;
        this.estado = estado;
    }

    public int getNSns() {
        return this.nSns;
    }

    public void setNSns(int nSns) {
        this.nSns = nSns;
    }

    public String getLocal() {
        return this.local;
    }

    public void setLocal(String local) {
        this.local = local;
    }

    public Date getHora_marcacao() {
        return this.hora_marcacao;
    }

    public void setHora_marcacao(Date hora_marcacao) {
        this.hora_marcacao = hora_marcacao;
    }

    public String getEstado() {
        return this.estado;
    }

    public void setEstado(String estado) {
        this.estado = estado;
    }

    @Override
    public String toString() {
        return getNSns() + "," + getLocal() + "," + getHora_marcacao() + "," + getEstado();
    }
}