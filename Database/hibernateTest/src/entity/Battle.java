package entity;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
public class Battle {
    private int winnerId;
    private int loserId;
    private Timestamp fightTime;

    @Basic
    @Column(name = "Winner_ID", nullable = false)
    public int getWinnerId() {
        return winnerId;
    }

    public void setWinnerId(int winnerId) {
        this.winnerId = winnerId;
    }

    @Basic
    @Column(name = "Loser_ID", nullable = false)
    public int getLoserId() {
        return loserId;
    }

    public void setLoserId(int loserId) {
        this.loserId = loserId;
    }

    @Basic
    @Column(name = "FightTime", nullable = false)
    public Timestamp getFightTime() {
        return fightTime;
    }

    public void setFightTime(Timestamp fightTime) {
        this.fightTime = fightTime;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Battle battle = (Battle) o;
        return winnerId == battle.winnerId &&
                loserId == battle.loserId &&
                Objects.equals(fightTime, battle.fightTime);
    }

    @Override
    public int hashCode() {
        return Objects.hash(winnerId, loserId, fightTime);
    }
}
