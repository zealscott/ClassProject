package entity;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import java.util.Objects;

@Entity
public class TreasureCollection {
    private int playerId;
    private String name;
    private Object type;
    private Integer attribution;
    private Boolean wear;

    @Id
    @Column(name = "PlayerID", nullable = false)
    public int getPlayerId() {
        return playerId;
    }

    public void setPlayerId(int playerId) {
        this.playerId = playerId;
    }

    @Basic
    @Column(name = "Name", nullable = true, length = 10)
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Basic
    @Column(name = "Type", nullable = true)
    public Object getType() {
        return type;
    }

    public void setType(Object type) {
        this.type = type;
    }

    @Basic
    @Column(name = "Attribution", nullable = true)
    public Integer getAttribution() {
        return attribution;
    }

    public void setAttribution(Integer attribution) {
        this.attribution = attribution;
    }

    @Basic
    @Column(name = "Wear", nullable = true)
    public Boolean getWear() {
        return wear;
    }

    public void setWear(Boolean wear) {
        this.wear = wear;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        TreasureCollection that = (TreasureCollection) o;
        return playerId == that.playerId &&
                Objects.equals(name, that.name) &&
                Objects.equals(type, that.type) &&
                Objects.equals(attribution, that.attribution) &&
                Objects.equals(wear, that.wear);
    }

    @Override
    public int hashCode() {
        return Objects.hash(playerId, name, type, attribution, wear);
    }
}
