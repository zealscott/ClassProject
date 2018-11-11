package entity;

import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;

@Entity
@Table(name="\"Player\"")
public class PlayerEntity {

    private Integer playerID;
    private Integer coin;
    private String playerName;

    @Id
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    @Column(name = "\"PlayerID\"", unique = true,nullable=false)
    public Integer getPlayerId() {
        return playerID;
    }

    public void setPlayerId(Integer playerId) {
        this.playerID = playerId;
    }

    @Column(name = "\"Name\"")
    public String getUserName() {
        return playerName;
    }

    public void setUserName(String playerName) {
        this.playerName = playerName;
    }

    @Column(name = "\"Coin\"")
    public Integer getCoin() {
        return coin;
    }

    public void setCoin(Integer coin) {
        this.coin = coin;
    }


}